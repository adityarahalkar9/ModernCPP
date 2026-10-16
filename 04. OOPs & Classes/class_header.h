#pragma once

int BasicClass();
int Constructors();
int Destructors();
int Inheritance();
int OperatorOverloading();
int VirtualFuction();
int Interfaces();
int Polymorphism();

// File 10 - Copy Semantics
void CopyConstructorDemo();
void ShallowVsDeepCopy();
void SelfAssignment();
void CopyAndSwapIdiom();
void RuleOfThree();

// File 11 - Move Semantics
void MoveConstructorDemo();
void MoveVsCopyPerformance();
void MoveInContainers();
void NoExceptImportance();
void MovedFromState();
void RuleOfFive();
void RuleOfZero();

// File 12 - Virtual Destructors
void WithoutVirtualDestructor();
void WithVirtualDestructor();
void SmartPointerVirtualDestructor();
void ProtectedDestructorPattern();
void WhenVirtualDestructor();
void VirtualDestructorCost();

// File 13 - Abstract Classes
void AbstractClassBasics();
void PartialAbstraction();
void InterfacePattern();
void PureVirtualWithImpl();
void AbstractInstantiation();

// File 14 - Multiple Inheritance
void MultipleInheritanceBasics();
void DiamondProblem();
void VirtualFunctionMI();
void AmbiguityResolution();
void MultipleInterfaces();

// File 15 - Virtual Inheritance
void VirtualInheritanceBasics();
void VirtualVsNonVirtual();
void VirtualInheritanceConstructors();
void VirtualInheritanceLayout();
void IostreamExample();

// File 16 - Object Slicing
void SlicingDemo();
void SlicingInContainers();
void SlicingInFunctions();
void AssignmentSlicing();
void IntentionalSlicing();
void PreventingSlicing();

// File 17 - Static Members
void StaticDataMembers();
void StaticMemberFunctions();
void SingletonPattern();
void StaticConstants();
void StaticInTemplates();
void ThreadSafeStatics();
void TrackingWithStatic();

// File 18 - Const Correctness
void ConstMemberFunctions();
void MutableMembers();
void ConstThisPointer();
void ReturningConstRefs();
void LogicalVsBitwiseConst();
void ConstPropagation();

// File 19 - Friend Functions
void FriendFunctions();
void FriendClasses();
void FriendMemberFunctions();
void OperatorOverloadingFriends();
void FriendForTesting();
void FriendshipRules();

// File 20 - Special Member Functions
void AllSpecialMembers();
void DefaultingSpecialMembers();
void DeletingSpecialMembers();
void GenerationRules();
void DetectionIdiom();
void RuleOfZeroExample();

// File 21 - Delegating Constructors
void DelegatingBasics();
void AvoidingDuplication();
void ChainingConstructors();
void InitVsDelegation();

// File 22 - Inheriting Constructors
void InheritingConstructorsBasics();
void WithoutInheritance();
void InheritanceWithExtra();
void WhenNotInherited();
void PracticalUse();

// File 23 - Rvalue/Lvalue
void BasicValueCategories();
void MemberFunctionsOnRValues();
void MoveAwareClass();
void ReturningValues();
void PerfectForwardingBasics();

// File 24 - Perfect Forwarding
void WithoutForwardingDemo();
void WithForwardingDemo();
void ForwardingToMembers();
void MakeFunctions();
void EmplaceForwarding();
void WhyForwardNeeded();

// File 25 - Covariant Returns
void CovariantReturnBasics();
void CovariantWithSmartPointers();
void CovariantWithReferences();
void ClonePattern();
void CovariantRules();

// File 26 - final
void FinalOnVirtualFunction();
void FinalOnClass();
void FinalWithOverride();
void FinalPerformance();
void WhenToUseFinal();

// File 27 - override
void WithoutOverrideDemo();
void WithOverrideDemo();
void CommonMistakes();
void OverrideVsVirtual();

// File 28 - NVI Pattern
void NVIBasics();
void NVIWithValidation();
void NVITemplateMethod();
void NVIBenefits();

// File 29 - CRTP
void CRTPBasics();
void StaticPolymorphism();
void CRTPMixins();
void CRTPPerformance();
void CRTPObjectCounting();

// File 30 - Type Erasure
void TypeErasureBasics();
void StdFunctionTypeErasure();
void MultiOperationTypeErasure();
void TypeErasureLimitations();

// File 31 - PIMPL Classes
void PimplBasics();
void PimplBenefits();
void FastPimpl();
void PimplSharedState();
void WhenToUsePimpl();

// File 32 - Factory Pattern
void SimpleFactoryDemo();
void FactoryMethodDemo();
void AbstractFactoryDemo();
void RegistryFactoryDemo();
void ModernFactoryDemo();

// File 33 - Composition Patterns
void CompositionVsInheritance();
void StrategyPattern();
void DecoratorPattern();
void ObserverPattern();

// File 34 - Mixin Pattern
void MixinBasics();
void PolicyMixins();
void CompositionMixins();
void MixinBenefits();

// File 35 - Concepts Classes
void ConceptConstrainedClass();
void InheritanceConcepts();
void RequiresClause();
void ConceptOverloading();
void PolymorphicConcepts();

// File 36 - Deducing this
void DeducingThisBasics();
void ChainingWithDeducingThis();
void DeduplicatingCode();
void SfinaeFree();

// File 37 - Static Operator
void StaticOperatorBasics();
void StatelessFunctorDemo();
void StaticSubscript();
void StaticOperatorBenefits();

// File 38 - Object Model
void VtableDemo();
void LayoutInspection();
void ConstructionOrder();
void MultipleVtables();
void EmptyBaseOpt();

// File 39 - Polymorphic Destruction
void ProperDestruction();
void DestructionOrder();
void RAIIWithPolymorphism();
void ExceptionSafety();
void AntiPatterns();

// File 40 - Design Patterns
void SingletonPattern();
void BuilderPattern();
void AdapterPattern();
void FacadePattern();
void CommandPattern();
void StatePattern();