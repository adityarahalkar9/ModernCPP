#pragma once

int Pointers();
int References();
int Dynamic();
int Vector();

// New declarations - File 05
void MemorySegments();
void StackMemory();
void StackOverflowRisk();
void HeapMemory();
void StackVsHeapComparison();
void StructurePadding();
void TypeSizesAndAddresses();
void VirtualMemoryConcept();

// File 06
void BasicPointerArithmetic();
void TypedPointerArithmetic();
void ArrayPointerDuality();
void PointerComparisons();
void MultiDimensionalPointerArithmetic();
void UnsafePointerTricks();
void OnePastTheEnd();
void RawMemoryOperations();

// File 07
void BasicFunctionPointers();
void TypedefForFunctionPointers();
void FunctionPointersAsParams();
void FunctionPointerArrays();
void CallbackDemo();
void MemberFunctionPointers();
void StdFunctionDemo();
void ComparisonDemo();
void DispatchTableDemo();

// File 08
void DataMemberPointers();
void MemberFunctionPointers();
void MemberPointersInContainers();
void GenericMemberAccess();
void DynamicMemberSelection();
void MemberPointersWithInheritance();

// File 09
void BasicVoidPointers();
void GenericFunctions();
void ContextCallbackDemo();
void VoidPointerDangers();
void VoidPointersInAllocation();
void ComparingVoidPointers();

// File 10
void BasicAlignment();
void OverAlignment();
void AlignedAllocation();
void StructureAlignment();
void HardwareAlignment();
void SIMDAlignment();
void ArrayAlignment();
void PackedStructures();

// File 11
void BasicPlacementNew();
void PlacementNewOnHeap();
void ArrayPlacementNew();
void MemoryPoolDemo();
void UseCases();
void Dangers();

// File 12
void StdByteBasics();
void ByteOperations();
void ByteVsChar();
void MemoryManipulation();
void ByteInAPIs();
void ByteWithModernFeatures();

// File 13
void BasicBitCast();
void WhyBitCastIsSafer();
void ConstexprBitCast();
void PracticalExamples();
void BitCastWithCustomTypes();
void ComparingBitPatterns();

// File 14
void BasicEndian();
void ManualEndianDetection();
void ByteSwapping();
void NetworkByteOrder();
void EndianIndependentCode();
void EndianWithBitCast();

// File 15
void FixedPoolDemo();
void ArenaDemo();
void ObjectPoolDemo();
void AllocationComparison();

// File 16
void DefaultAllocator();
void CustomAllocatorDemo();
void PoolAllocatorDemo();
void AllocatorRequirements();

// File 17
void PMRIntroduction();
void PMRVectorDemo();
void MonotonicBufferDemo();
void PoolResourceDemo();
void NestedContainersDemo();
void CustomResourceDemo();
void PMRBestPractices();

// File 18
void DanglingPointers();
void DoubleFree();
void UseAfterFree();
void BufferOverflow();
void MemoryLeaks();
void SmartPointerSolutions();
void IteratorInvalidation();
void BestPracticesSummary();

// File 19
void BasicLifetime();
void TemporaryLifetimes();
void ContainerLifetimes();
void OptionalLifetime();
void DynamicLifetime();
void LifetimeIssues();
void ConstructionOrder();

// File 20
void OwnershipModels();
void ExclusiveOwnership();
void SharedOwnership();
void NonOwningReferences();
void OwnershipInContainers();
void OwnershipTransfer();
void OwnershipBestPractices();

// File 21
void UniquePtrBasics();
void UniquePtrCustomDeleter();
void SharedPtrBasics();
void SharedPtrAliasing();
void WeakPtrBasics();
void BreakingCycles();
void EnableSharedFromThis();
void PerformanceNotes();

// File 22
void ClassSpecificNewDelete();
void PlacementNewDemo();
void AlignedNewDelete();
void SizedDeleteDemo();
void UseCases();
void CustomAllocatorDemo();

// File 23
void BasicLeak();
void LostPointerLeak();
void ExceptionLeak();
void ContainerLeak();
void LeakDetection();
void RAIISolution();
void SharedPtrCycle();

// File 24
void OutPtrBasics();
void InOutPtrBasics();
void PracticalExample();
void Comparison();

// File 25
void AllocateAtLeastBasics();
void VectorOptimization();
void CustomAllocatorDemo();

// File 26
void StartLifetimeAsBasics();
void UseCases();
void ImplicitLifetimeTypes();
void Demonstration();
void Comparison();

// File 27
void LaunderBasics();
void ClassicUseCase();
void LaunderWithConst();
void WhenNotNeeded();
void ManualOptionalDemo();

// File 28
void AlignedStorageBasics();
void MyOptionalDemo();
void DeprecationNotice();

// File 29
void SmallStringDemo();
void StdStringSBO();
void FunctionStorageDemo();
void SBOBenefits();

// File 30
void SoAvsAoS();
void CacheFriendlyIteration();
void FalseSharingDemo();
void PrefetchingDemo();
void DataLayoutOptimization();
void AccessPatterns();