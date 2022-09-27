/********************************************************************
* TianGong RenderLab												*
* Copyright (c) Gaiyitp9. All rights reserved.						*
* This code is licensed under the MIT License (MIT).				*
*																	*
* Noted:															*
* This file is part of Eigen, a lightweight C++ template library	*
* for linear algebra which is subject to the terms of the			*
* Mozilla Public License v.2.0. And I made some simplifications.	*
*********************************************************************/
#pragma once

namespace LCH::Math
{

template<typename DstEvaluator, typename SrcEvaluator, typename AssignFunc>
struct copy_using_evaluator_traits
{
	using Dst = DstEvaluator::XprType;
	using DstScalar = Dst::Scalar;

	constexpr static Flag DstFlags = DstEvaluator::Flags;
	constexpr static Flag SrcFlags = SrcEvaluator::Flags;

	constexpr static int DstAlignment = DstEvaluator::Alignment;
	constexpr static int SrcAlignment = SrcEvaluator::Alignment;
	constexpr static int JointAlignment = DstAlignment <= SrcAlignment ? DstAlignment : SrcAlignment;

	constexpr static int InnerSize = Dst::IsVectorAtCompileTime ? Dst::SizeAtCompileTime :
									 not_none(DstFlags & Flag::RowMajor) ? Dst::ColsAtCompileTime : Dst::RowsAtCompileTime;
	constexpr static int LinearSize = Dst::SizeAtCompileTime;

	using LinearPacketType = best_packet<DstScalar, LinearSize>;
	using InnerPacketType = best_packet<DstScalar, InnerSize>;
	constexpr static int LinearPacketSize = unpacket_traits<LinearPacketType>::Size;
	constexpr static int InnerPacketSize = unpacket_traits<InnerPacketType>::Size;
	constexpr static int LinearRequiredAlignment = unpacket_traits<LinearPacketType>::Alignment;
	constexpr static int InnerRequiredAlignment = unpacket_traits<InnerPacketType>::Alignment;

	constexpr static bool DstIsRowMajor = not_none(DstFlags & Flag::RowMajor);
	constexpr static bool SrcIsRowMajor = not_none(SrcFlags & Flag::RowMajor);
	constexpr static bool StorageOrdersAgree = DstIsRowMajor == SrcIsRowMajor;
	constexpr static bool MayVectorize = StorageOrdersAgree && not_none(DstFlags & SrcFlags & Flag::PacketAccess)
											&& functor_traits<AssignFunc>::PacketAccess;
	constexpr static bool MayLinearize = StorageOrdersAgree && not_none(DstFlags & SrcFlags & Flag::LinearAccess);
	constexpr static bool MayInnerVectorize = MayVectorize && InnerSize != Dynamic && InnerSize % InnerPacketSize == 0;
	constexpr static bool MayLinearVectorize = MayVectorize && MayLinearize;

	constexpr static TraversalType Traversal = Dst::SizeAtCompileTime == 0 ? TraversalType::AllAtOnce 
									: MayLinearVectorize && (LinearPacketSize >= InnerPacketSize) ? TraversalType::LinearVectorized
									: MayInnerVectorize ? TraversalType::InnerVectorized
									: MayLinearVectorize ? TraversalType::LinearVectorized
									: MayLinearize ? TraversalType::Linear : TraversalType::Default;
	constexpr static bool Vectorized = Traversal == TraversalType::InnerVectorized ||
									Traversal == TraversalType::LinearVectorized;

	using PacketType = std::conditional_t<Traversal == TraversalType::LinearVectorized, LinearPacketType, InnerPacketType>;

	constexpr static int ActualPacketSize = Traversal == TraversalType::LinearVectorized ? LinearPacketSize
									: Vectorized ? InnerPacketSize : 1;
	constexpr static bool MayUnrollCompletely = LinearSize != Dynamic;
	constexpr static bool MayUnrollInner = InnerSize != Dynamic;

	constexpr static UnrollingType Unrolling = MayUnrollCompletely ? UnrollingType::Complete : UnrollingType::None;
};

template<typename Kernel, int Index, int Stop>
struct DefaultTraversalCompleteUnrolling
{
	using DstXprType = Kernel::DstXprType;
	constexpr static int outer = Index / DstXprType::InnerSizeAtCompileTime;
	constexpr static int inner = Index % DstXprType::InnerSizeAtCompileTime;

	static void Run(Kernel& kernel)
	{
		kernel.AssignCoeffByOuterInner(outer, inner);
		DefaultTraversalCompleteUnrolling<Kernel, Index + 1, Stop>::Run(kernel);
	}
};

template<typename Kernel, int Stop>
struct DefaultTraversalCompleteUnrolling<Kernel, Stop, Stop>
{
	static void Run(Kernel&) {}
};

template<typename Kernel, int Index, int Stop>
struct LinearTraversalCompleteUnrolling
{
	static void Run(Kernel& kernel)
	{
		kernel.AssignCoeff(Index);
		LinearTraversalCompleteUnrolling<Kernel, Index + 1, Stop>::Run(kernel);
	}
};

template<typename Kernel, int Stop>
struct LinearTraversalCompleteUnrolling<Kernel, Stop, Stop>
{
	static void Run(Kernel&) {}
};

template<typename Kernel, int Index, int Stop>
struct InnerVectorizedTraversalCompleteUnrolling
{
	using DstXprType = Kernel::DstXprType;
	using PacketType = Kernel::PacketType;

	constexpr static int outer = Index / DstXprType::InnerSizeAtCompileTime;
	constexpr static int inner = Index % DstXprType::InnerSizeAtCompileTime;
	constexpr static int SrcAlignment = Kernel::AssignmentTraits::SrcAlignment;
	constexpr static int DstAlignment = Kernel::AssignmentTraits::DstAlignment;

	static void Run(Kernel& kernel)
	{
		kernel.template AssignPacketByOuterInner<DstAlignment, SrcAlignment, PacketType>(outer, inner);
		constexpr static int NextIndex = Index + unpacket_traits<PacketType>::Size;
		InnerVectorizedTraversalCompleteUnrolling<Kernel, NextIndex, Stop>::Run(kernel);
	}
};

template<typename Kernel, int Stop>
struct InnerVectorizedTraversalCompleteUnrolling<Kernel, Stop, Stop>
{
	static void Run(Kernel&) {}
};

template<typename Kernel,
	TraversalType Traversal = Kernel::AssignmentTraits::Traversal,
	UnrollingType Unrolling = Kernel::AssignmentTraits::Unrolling>
struct assignment_loop;

template<typename Kernel, UnrollingType Unrolling>
struct assignment_loop<Kernel, TraversalType::AllAtOnce, Unrolling>
{
	static void Run(Kernel&) {}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Default, UnrollingType::None>
{
	static void Run(Kernel& kernel)
	{
		for (int outer = 0; outer < kernel.outerSize(); ++outer)
		{
			for (int inner = 0; inner < kernel.innerSize(); ++inner)
			{
				kernel.AssignCoeffByOuterInner(outer, inner);
			}
		}
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Default, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstEvaluatorType::XprType;
		DefaultTraversalCompleteUnrolling<Kernel, 0, DstXprType::SizeAtCompileTime>::Run(kernel);
	}
};

template<bool IsAligned = false>
struct unaligned_assignment_loop
{
	template<typename Kernel>
	static void Run(Kernel&, int, int) {}
};

template<>
struct unaligned_assignment_loop<false>
{
	template<typename Kernel>
	static void Run(Kernel& kernel, int start, int end)
	{
		for (int index = start; index < end; ++index)
			kernel.AssignCoeff(index);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::LinearVectorized, UnrollingType::None>
{
	static void Run(Kernel& kernel)
	{
		const int size = kernel.size();
		using Scalar = Kernel::Scalar;
		using PacketType = Kernel::PacketType;
		constexpr static int requestedAlignment = Kernel::AssignmentTraits::LinearRequiredAlignment;
		constexpr static int packetSize = unpacket_traits<PacketType>::Size;
		// 下面三行代码用来计算Dst和Src的对齐，一般情况下Dst和Src的对齐是相同的，下面的计算相当于把DstAlignment
		// 和SrcAlignment分别设置为Kernel::AssignmentTraits::DstAlignment和Kernel::AssignmentTraits::SrcAlignment;
		// 考虑Block这种特殊情况，Dst和Src的对齐可能不相同，所以，如果DstAlignment可以根据下面的first_aligned推导出，
		// SrcAlignment要取上面两个对齐的最小值，Dst或者Src是Block都可能造成不对齐，所以取最小值
		constexpr static bool dstIsAligned = Kernel::AssignmentTraits::DstAlignment >= requestedAlignment;
		constexpr static int dstAlignment = packet_traits<Scalar>::AlignedOnScalar ? requestedAlignment 
																				   : Kernel::AssignmentTraits::DstAlignment;
		constexpr static int srcAlignment = Kernel::AssignmentTraits::JointAlignment;

		const int alignedStart = dstIsAligned ? 0 : first_aligned<requestedAlignment>(kernel.dstDataPtr(), size);
		const int alignedEnd = alignedStart + ((size - alignedStart) / packetSize) * packetSize;

		unaligned_assignment_loop<dstIsAligned>::Run(kernel, 0, alignedStart);

		for (int index = alignedStart; index < alignedEnd; index += packetSize)
			kernel.template AssignPacket<dstAlignment, srcAlignment, PacketType>(index);

		unaligned_assignment_loop<>::Run(kernel, alignedEnd, size);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::LinearVectorized, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstXprType;
		using PacketType = Kernel::PacketType;

		constexpr static int size = DstXprType::SizeAtCompileTime;
		constexpr static int packetSize = unpacket_traits<PacketType>::Size;
		constexpr static int alignedSize = (size / packetSize) * packetSize;

		InnerVectorizedTraversalCompleteUnrolling<Kernel, 0, alignedSize>::Run(kernel);
		DefaultTraversalCompleteUnrolling<Kernel, alignedSize, size>::Run(kernel);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::InnerVectorized, UnrollingType::None>
{
	using PacketType = Kernel::PacketType;
	constexpr static int SrcAlignment = Kernel::AssignmentTraits::SrcAlignment;
	constexpr static int DstAlignment = Kernel::AssignmentTraits::DstAlignment;

	static void Run(Kernel& kernel)
	{
		const int innerSize = kernel.innerSize();
		const int outerSize = kernel.outerSize();
		const int packetSize = unpacket_traits<PacketType>::size;
		for (int outer = 0; outer < outerSize; ++outer)
			for (int inner = 0; inner < innerSize; inner += packetSize)
				kernel.template assignPacketByOuterInner<DstAlignment, SrcAlignment, PacketType>(outer, inner);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::InnerVectorized, UnrollingType::Complete>
{
	static void Run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstXprType;
		InnerVectorizedTraversalCompleteUnrolling<Kernel, 0, DstXprType::SizeAtCompileTime>::Run(kernel);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Linear, UnrollingType::None>
{
	static void run(Kernel& kernel)
	{
		const int size = kernel.size();
		for (int i = 0; i < size; ++i)
			kernel.AssignCoeff(i);
	}
};

template<typename Kernel>
struct assignment_loop<Kernel, TraversalType::Linear, UnrollingType::Complete>
{
	static void run(Kernel& kernel)
	{
		using DstXprType = Kernel::DstXprType;
		LinearTraversalCompleteUnrolling<Kernel, 0, DstXprType::SizeAtCompileTime>::Run(kernel);
	}
};

// 赋值类
template<typename DstXprType, typename SrcXprType, typename Functor>
struct Assignment;

template<typename DstEvaluatorTypeT, typename SrcEvaluatorTypeT, typename Functor>
class generic_assignment_kernel
{
public:
	using DstXprType = DstEvaluatorTypeT::XprType;
	using DstEvaluatorType = DstEvaluatorTypeT;
	using SrcEvaluatorType = SrcEvaluatorTypeT;
	using Scalar = DstEvaluatorType::Scalar;
	using AssignmentTraits = copy_using_evaluator_traits<DstEvaluatorType, SrcEvaluatorType, Functor>;
	using PacketType = AssignmentTraits::PacketType;

	generic_assignment_kernel(DstEvaluatorType& dst, const SrcEvaluatorType& src, const Functor& func, DstXprType& dstExpr)
		: m_dst(dst), m_src(src), m_functor(func), m_dstExpr(dstExpr)
	{}

	constexpr int size() const { return m_dstExpr.size(); }
	constexpr int innerSize() const { return m_dstExpr.innerSize(); }
	constexpr int outerSize() const { return m_dstExpr.outerSize(); }
	constexpr int rows() const { return m_dstExpr.rows(); }
	constexpr int cols() const { return m_dstExpr.cols(); }
	constexpr int outerStride() const { return m_dstExpr.outerStride(); }

	DstEvaluatorType& dstEvaluator() noexcept { return m_dst; }
	const SrcEvaluatorType& srcEvaluator() noexcept { return m_src; }

	void AssignCoeff(int row, int col)
	{
		m_functor.AssignCoeff(m_dst.coeffRef(row, col), m_src.coeff(row, col));
	}

	void AssignCoeff(int index)
	{
		m_functor.AssignCoeff(m_dst.coeffRef(index), m_src.coeff(index));
	}

	void AssignCoeffByOuterInner(int outer, int inner)
	{
		int row = rowIndexByOuterInner(outer, inner);
		int col = colIndexByOuterInner(outer, inner);
		AssignCoeff(row, col);
	}

	template<int StoreMode, int LoadMode, typename PacketType>
	void AssignPacket(int row, int col)
	{
		m_functor.template AssignPacket<StoreMode>(&m_dst.coeffRef(row, col), m_src.template packet<LoadMode, PacketType>(row, col));
	}

	template<int StoreMode, int LoadMode, typename PacketType>
	void AssignPacket(int index)
	{
		m_functor.template AssignPacket<StoreMode>(&m_dst.coeffRef(index), m_src.template packet<LoadMode, PacketType>(index));
	}

	template<int StoreMode, int LoadMode, typename PacketType>
	void AssignPacketByOuterInner(int outer, int inner)
	{
		int row = rowIndexByOuterInner(outer, inner);
		int col = colIndexByOuterInner(outer, inner);
		AssignPacket<StoreMode, LoadMode, PacketType>(row, col);
	}

	static int rowIndexByOuterInner(int outer, int inner)
	{
		return DstEvaluatorType::RowsAtCompileTime == 1 ? 0 :
			DstEvaluatorType::ColsAtCompileTime == 1 ? inner :
			not_none(DstEvaluatorType::Flags & Flag::RowMajor) ? outer : inner;
	}

	static int colIndexByOuterInner(int outer, int inner)
	{
		return DstEvaluatorType::ColsAtCompileTime == 1 ? 0 :
			DstEvaluatorType::RowsAtCompileTime == 1 ? inner :
			not_none(DstEvaluatorType::Flags & Flag::RowMajor) ? inner : outer;
	}

	const Scalar* dstDataPtr() const
	{
		return m_dstExpr.data();
	}

protected:
	DstEvaluatorType& m_dst;
	const SrcEvaluatorType& m_src;
	const Functor& m_functor;
	DstXprType& m_dstExpr;
};

template<typename DstXprType, typename SrcXprType, typename Functor>
struct Assignment
{
	static void Run(DstXprType& dst, const SrcXprType& src, const Functor& func)
	{
		using DstEvaluatorType = evaluator<DstXprType>;
		using SrcEvaluatorType = evaluator<SrcXprType>;
		SrcEvaluatorType srcEvaluator(src);
		DstEvaluatorType dstEvaluator(dst);

		using Kernel = generic_assignment_kernel<DstEvaluatorType, SrcEvaluatorType, Functor>;
		Kernel kernel(dstEvaluator, srcEvaluator, func, dst.const_cast_derived());

		assignment_loop<Kernel>::Run(kernel);
	}
};


template<typename Dst, typename Src, typename Func>
void call_assignment_no_alias(Dst& dst, const Src& src, const Func& func)
{
	constexpr static bool NeedToTranspose =
		(Dst::RowsAtCompileTime == 1 && Src::ColsAtCompileTime == 1) ||
		(Dst::ColsAtCompileTime == 1 && Src::RowsAtCompileTime == 1) &&
		Dst::SizeAtCompileTime != 1;

	using ActualDstTypeCleaned = std::conditional_t<NeedToTranspose, Transpose<Dst>, Dst>;
	using ActualDstType = std::conditional_t<NeedToTranspose, Transpose<Dst>, Dst&>;
	ActualDstType actualDst(dst);

	static_assert(is_lvalue<Dst>, "The expression is not a left value.");
	static_assert(have_same_matrix_size<ActualDstTypeCleaned, Src>, "You mixed matrices of different sizes.");
	Assignment<ActualDstTypeCleaned, Src, Func>::Run(actualDst, src, func);
}

template<typename Dst, typename Src, typename Func>
void call_assignment(Dst& dst, Src& src, const Func& func)
{
	using type = Matrix<typename traits<Src>::Scalar,
		traits<Src>::RowsAtCompileTime,
		traits<Src>::ColsAtCompileTime,
		not_none(traits<Src>::Flags & Flag::RowMajor) ? StorageOption::RowMajor : StorageOption::ColMajor
	>;

	if constexpr (evaluator_assume_aliasing<Src>::value)
	{
		type tmp(src);
		call_assignment_no_alias(dst, tmp, func);
	}
	else
		call_assignment_no_alias(dst, src, func);
}

template<typename Dst, typename Src>
void call_assignment(Dst& dst, const Src& src)
{
	call_assignment(dst, src, assign_op<typename Dst::Scalar, typename Src::Scalar>());
}

}