import std;
#include "smart_pointers_header.h"

int main() {
    std::cout << " 05. Smart Pointers Complete" << std::endl;

    // --- unique_ptr Basics (01) ---
    // UniquePtrCreation();
    // UniquePtrAccess();
    // UniquePtrTransfer();
    // UniquePtrReset();
    // UniquePtrRelease();
    // UniquePtrSwap();

    // --- unique_ptr Advanced (02) ---
    // UniquePtrFromFactory();
    // UniquePtrToFunctions();
    // UniquePtrInContainers();
    // UniquePtrPolymorphism();
    // ConditionalOwnership();
    // ScopeGuardPattern();

    // --- Custom Deleters (03) ---
    // UniquePtrWithFunctionDeleter();
    // UniquePtrWithLambdaDeleter();
    // UniquePtrWithFunctorDeleter();
    // UniquePtrWithMallocDeleter();
    // UniquePtrWithNoOpDeleter();
    // UsingTypeAliases();
    // DeleterSizeOverhead();

    // --- Arrays (04) ---
    // UniquePtrArrayBasics();
    // UniquePtrArrayWithValues();
    // UniquePtr2DArray();
    // UniquePtrFlattened2D();
    // UniquePtrArrayPitfalls();
    // ArrayVsVector();

    // --- shared_ptr Basics (05) ---
    // SharedPtrCreation();
    // SharedPtrCopying();
    // SharedPtrOwnership();
    // SharedPtrAssignment();
    // SharedPtrReset();
    // SharedPtrSwap();
    // SharedPtrInContainers();

    // --- shared_ptr Advanced (06) ---
    // SharedPtrCustomDeleter();
    // SharedFromUnique();
    // MultipleOwnership();
    // SharedPtrInClasses();
    // ReferenceCountingDetails();
    // SharedPtrComparison();
    // SharedPtrFactory();

    // --- Control Block (07) ---
    // ControlBlockBasics();
    // MakeSharedVsNew();
    // ControlBlockLifetime();
    // StrongVsWeakCounts();
    // CustomAllocatorControlBlock();

    // --- Aliasing (08) ---
    // AliasingConstructorBasics();
    // AliasingForMembers();
    // AliasingWithArrays();
    // AliasingForPolymorphism();
    // AliasingUseCases();

    // --- weak_ptr (09) ---
    // WeakPtrCreation();
    // WeakPtrLock();
    // WeakPtrExpired();
    // WeakPtrUseCount();
    // WeakPtrReset();
    // WeakPtrInClasses();
    // WeakPtrSwap();

    // --- Cycles (10) ---
    // CycleProblem();
    // CycleSolution();
    // ParentChildPattern();
    // ObserverPatternWeak();
    // CycleDetection();

    // --- enable_shared_from_this (11) ---
    // EnableSharedBasics();
    // SharedFromThisInside();
    // AsyncUseCase();
    // EnableSharedRules();
    // WeakFromThisDemo();

    // --- make_unique (12) ---
    // MakeUniqueBasics();
    // MakeUniqueVariants();
    // MakeUniqueExceptionSafety();
    // MakeUniqueWithCustomTypes();
    // MakeUniquePerformance();
    // MakeUniqueGuidelines();

    // --- make_shared (13) ---
    // MakeSharedBasics();
    // MakeSharedAllocation();
    // MakeSharedLifetime();
    // MakeSharedWithArgs();
    // MakeSharedArrays();
    // MakeSharedBestPractices();

    // --- Containers (14) ---
    // UniquePtrInVector();
    // SharedPtrInVector();
    // SmartPointersInMaps();
    // SmartPointersInSets();
    // MovingSmartPointers();
    // ContainerOfSmartPointersPatterns();

    // --- Polymorphism (15) ---
    // PolymorphicUniquePtr();
    // PolymorphicSharedPtr();
    // PolymorphicContainers();
    // PolymorphicFactory();
    // DowncastingSmartPointers();

    // --- Functions (16) ---
    // UniquePtrParameters();
    // UniquePtrReturn();
    // SmartPtrWithStdFunction();
    // CallbacksWithSmartPointers();

    // --- Exceptions (17) ---
    // ExceptionSafety();
    // ExceptionDuringConstruction();
    // CleanupWithExceptions();
    // StrongExceptionGuarantee();
    // NoexceptDestructors();

    // --- RAII (18) ---
    // RAIIBasics();
    // RAIIWithExceptions();
    // RAIIFileExample();
    // RAIIMutexExample();
    // CustomRAIIWrapper();
    // RAIIDesignPrinciples();

    // --- Scope Guards (19) ---
    // BasicScopeGuard();
    // ScopeGuardWithDismiss();
    // TransactionScopeGuard();
    // ConditionalScopeGuards();
    // DeferPattern();

    // --- Ownership (20) ---
    // OwnershipSemantics();
    // TransferOwnership();
    // FactoryOwnership();
    // UniquePtrContainers();
    // ReferenceVsOwnership();

    // --- Thread Safety (21) ---
    // SharedPtrAtomicity();
    // SharedPtrAcrossThreads();
    // RaceConditionDemo();
    // ProperSynchronization();
    // AtomicSharedPtrSafety();

    // --- Atomic (22) ---
    // AtomicSharedPtrConcept();
    // SimulatedAtomicSharedPtr();
    // ProducerConsumerPattern();
    // RCUPattern();
    // AtomicSharedPtrBestPractices();

    // --- Pitfalls (23) ---
    // PitfallSharedThis();
    // PitfallCycles();
    // PitfallDangling();
    // PitfallMixed();
    // PitfallGetMisuse();
    // PitfallThreading();
    // PitfallOveruse();

    // --- Performance (24) ---
    // UniquePtrOverhead();
    // SharedPtrOverhead();
    // BenchmarkCreation();
    // BenchmarkCopying();
    // PerformanceGuidelines();

    // --- intrusive_ptr (25) ---
    // IntrusivePtrBasics();
    // IntrusivePtrUsage();
    // IntrusiveVsShared();

    // --- Conversions (26) ---
    // UniqueToShared();
    // StaticPointerCast();
    // DynamicPointerCast();
    // ConstPointerCast();
    // ReinterpretPointerCast();
    // ConversionBestPractices();

    // --- Comparisons (27) ---
    // ComparingPointerIdentity();
    // ComparingPointedToValues();
    // SmartPointersInMap();
    // SmartPointersInSet();
    // HashingSmartPointers();

    // --- pointer_traits (28) ---
    // PointerTraitsBasics();
    // TypeTraitsForSmartPointers();
    // DetectingSmartPointer();
    // GenericCodeWithSmartPointers();
    // MakingCustomPointerLike();

    // --- Antipatterns (29) ---
    // AntipatternOveruse();
    // AntipatternGetDelete();
    // AntipatternSharedFromRaw();
    // AntipatternNoMake();
    // AntipatternWrongType();
    // AntipatternCycles();
    // AntipatternSummary();

    // --- Best Practices (30) ---
    // BestPracticesSummary();
    // OwnershipGuidelines();
    // APIDesignGuidelines();
    // CommonPatterns();
    // DecisionTree();
    // MigrationGuide();

    return 0;
}