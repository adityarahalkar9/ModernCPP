#pragma once

// File 01 - unique_ptr basics
void UniquePtrCreation();
void UniquePtrAccess();
void UniquePtrTransfer();
void UniquePtrReset();
void UniquePtrRelease();
void UniquePtrSwap();

// File 02 - unique_ptr advanced
void UniquePtrFromFactory();
void UniquePtrToFunctions();
void UniquePtrInContainers();
void UniquePtrPolymorphism();
void ConditionalOwnership();
void ScopeGuardPattern();

// File 03 - unique_ptr custom deleters
void UniquePtrWithFunctionDeleter();
void UniquePtrWithLambdaDeleter();
void UniquePtrWithFunctorDeleter();
void UniquePtrWithMallocDeleter();
void UniquePtrWithNoOpDeleter();
void UsingTypeAliases();
void DeleterSizeOverhead();

// File 04 - unique_ptr arrays
void UniquePtrArrayBasics();
void UniquePtrArrayWithValues();
void UniquePtr2DArray();
void UniquePtrFlattened2D();
void UniquePtrArrayPitfalls();
void ArrayVsVector();

// File 05 - shared_ptr basics
void SharedPtrCreation();
void SharedPtrCopying();
void SharedPtrOwnership();
void SharedPtrAssignment();
void SharedPtrReset();
void SharedPtrSwap();
void SharedPtrInContainers();

// File 06 - shared_ptr advanced
void SharedPtrCustomDeleter();
void SharedFromUnique();
void MultipleOwnership();
void SharedPtrInClasses();
void ReferenceCountingDetails();
void SharedPtrComparison();
void SharedPtrFactory();

// File 07 - control block
void ControlBlockBasics();
void MakeSharedVsNew();
void ControlBlockLifetime();
void StrongVsWeakCounts();
void CustomAllocatorControlBlock();

// File 08 - aliasing
void AliasingConstructorBasics();
void AliasingForMembers();
void AliasingWithArrays();
void AliasingForPolymorphism();
void AliasingUseCases();

// File 09 - weak_ptr basics
void WeakPtrCreation();
void WeakPtrLock();
void WeakPtrExpired();
void WeakPtrUseCount();
void WeakPtrReset();
void WeakPtrInClasses();
void WeakPtrSwap();

// File 10 - cycles
void CycleProblem();
void CycleSolution();
void ParentChildPattern();
void ObserverPatternWeak();
void CycleDetection();

// File 11 - enable_shared_from_this
void EnableSharedBasics();
void SharedFromThisInside();
void AsyncUseCase();
void EnableSharedRules();
void WeakFromThisDemo();

// File 12 - make_unique
void MakeUniqueBasics();
void MakeUniqueVariants();
void MakeUniqueExceptionSafety();
void MakeUniqueWithCustomTypes();
void MakeUniquePerformance();
void MakeUniqueGuidelines();

// File 13 - make_shared
void MakeSharedBasics();
void MakeSharedAllocation();
void MakeSharedLifetime();
void MakeSharedWithArgs();
void MakeSharedArrays();
void MakeSharedBestPractices();

// File 14 - containers
void UniquePtrInVector();
void SharedPtrInVector();
void SmartPointersInMaps();
void SmartPointersInSets();
void MovingSmartPointers();
void ContainerOfSmartPointersPatterns();

// File 15 - polymorphism
void PolymorphicUniquePtr();
void PolymorphicSharedPtr();
void PolymorphicContainers();
void PolymorphicFactory();
void DowncastingSmartPointers();

// File 16 - functions
void UniquePtrParameters();
void UniquePtrReturn();
void SmartPtrWithStdFunction();
void CallbacksWithSmartPointers();

// File 17 - exceptions
void ExceptionSafety();
void ExceptionDuringConstruction();
void CleanupWithExceptions();
void StrongExceptionGuarantee();
void NoexceptDestructors();

// File 18 - RAII patterns
void RAIIBasics();
void RAIIWithExceptions();
void RAIIFileExample();
void RAIIMutexExample();
void CustomRAIIWrapper();
void RAIIDesignPrinciples();

// File 19 - scope guards
void BasicScopeGuard();
void ScopeGuardWithDismiss();
void TransactionScopeGuard();
void ConditionalScopeGuards();
void DeferPattern();

// File 20 - ownership
void OwnershipSemantics();
void TransferOwnership();
void FactoryOwnership();
void UniquePtrContainers();
void ReferenceVsOwnership();

// File 21 - thread safety
void SharedPtrAtomicity();
void SharedPtrAcrossThreads();
void RaceConditionDemo();
void ProperSynchronization();
void AtomicSharedPtrSafety();

// File 22 - atomic shared_ptr
void AtomicSharedPtrConcept();
void SimulatedAtomicSharedPtr();
void ProducerConsumerPattern();
void RCUPattern();
void AtomicSharedPtrBestPractices();

// File 23 - pitfalls
void PitfallSharedThis();
void PitfallCycles();
void PitfallDangling();
void PitfallMixed();
void PitfallGetMisuse();
void PitfallThreading();
void PitfallOveruse();

// File 24 - performance
void UniquePtrOverhead();
void SharedPtrOverhead();
void BenchmarkCreation();
void BenchmarkCopying();
void PerformanceGuidelines();

// File 25 - intrusive_ptr
void IntrusivePtrBasics();
void IntrusivePtrUsage();
void IntrusiveVsShared();

// File 26 - conversions
void UniqueToShared();
void StaticPointerCast();
void DynamicPointerCast();
void ConstPointerCast();
void ReinterpretPointerCast();
void ConversionBestPractices();

// File 27 - comparisons
void ComparingPointerIdentity();
void ComparingPointedToValues();
void SmartPointersInMap();
void SmartPointersInSet();
void HashingSmartPointers();

// File 28 - pointer_traits
void PointerTraitsBasics();
void TypeTraitsForSmartPointers();
void DetectingSmartPointer();
void GenericCodeWithSmartPointers();
void MakingCustomPointerLike();

// File 29 - antipatterns
void AntipatternOveruse();
void AntipatternGetDelete();
void AntipatternSharedFromRaw();
void AntipatternNoMake();
void AntipatternWrongType();
void AntipatternCycles();
void AntipatternSummary();

// File 30 - best practices
void BestPracticesSummary();
void OwnershipGuidelines();
void APIDesignGuidelines();
void CommonPatterns();
void DecisionTree();
void MigrationGuide();