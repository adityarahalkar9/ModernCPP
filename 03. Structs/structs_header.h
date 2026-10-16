#pragma once

// File 01
void StructDefinition();
void StructWithDifferentTypes();
void DefaultMemberInitializers();
void StructCopying();
void StructsAsValueTypes();
void StructArrays();
void StructPointerAccess();

// File 02
void InitializationForms();
void DesignatedInitializers();
void NestedInitialization();
void InitializerLists();
void InitializationBestPractices();

// File 03
void MemoryLayoutBasics();
void AlignmentRules();
void EmptyStructSize();
void InheritanceLayout();
void VirtualFunctionLayout();
void ArrayContiguity();

// File 04
void DefaultAccessDifference();
void InheritanceAccessDifference();
void WhenToUse();
void AppropriateUsage();
void TechnicalEquivalence();

// File 05
void AggregateBasics();
void AggregateWithBase();
void ArrayAggregates();
void LosingAggregateStatus();
void PracticalImplications();

// File 06
void PODBasics();
void TrivialTypes();
void TriviallyCopyableTypes();
void PracticalUsage();
void CCompatibility();

// File 07
void StandardLayoutBasics();
void WhyStandardLayout();
void OffsetofDemo();
void ByteOffsetAccess();
void CommonPatterns();

// File 08
void BasicBitFields();
void FlagBitFields();
void PackedValues();
void HardwareRegisters();
void BitFieldPitfalls();
void ManualBitManipulation();

// File 09
void BasicMemberFunctions();
void OperatorOverloading();
void ConstMemberFunctions();
void StaticMemberFunctions();
void ThisPointer();

// File 10
void BasicInheritance();
void MultiLevelInheritance();
void MemberHiding();
void AggregateInheritance();
void CompositionVsInheritance();
void AccessLevels();

// File 11
void ManualComparisonOperators();
void DefaultedComparison();
void CustomThreeWayComparison();
void ComparisonCategories();
void ComparisonInContainers();
void SortingWithComparison();

// File 12
void BasicStructuredBindings();
void BindingsWithMultipleMembers();
void BindingsInLoops();
void BindingsWithMaps();
void BindingsWithArrays();
void SwapWithBindings();
void BindingLimitations();

// File 13
void BasicAnonymousStruct();
void AnonymousUnion();
void TaggedUnion();
void NestedAnonymous();
void AnonymousStructInUnion();
void LimitationsAndWarnings();

// File 14
void BasicUnions();
void UnionWithNonTrivial();
void TaggedUnionPattern();
void MemorySavings();
void TypePunning();
void SaferAlternative();

// File 15
void PassingStructs();
void ReturningStructs();
void RVODemo();
void PerformanceComparison();
void DefaultParameters();

// File 16
void ConstexprStructs();
void ConstexprMatrix();
void CompileTimeDataGeneration();
void ConstevalStructs();
void ConstexprString();

// File 17
void BasicStructTemplates();
void MultipleTemplateParams();
void NonTypeTemplateParams();
void TemplateSpecialization();
void CRTPWithStructs();
void ConceptConstrainedStructs();

// File 18
void BasicTypeTraits();
void SizeAndAlignmentTraits();
void UsingTraitsInTemplates();
void CompileTimeChecks();
void ConditionalCompilation();

// File 19
void BasicNestedStructs();
void DeepNesting();
void NestedWithArrays();
void ScopedNestedStructs();
void NestedWithInheritance();
void DeepNestedAccess();

// File 20
void BadLayouts();
void OptimizedLayouts();
void SortBySize();
void PackedStructs();
void CustomAlignment();

// File 21
void BasicSerialization();
void VersionedSerialization();
void PacketSerialization();
void EndianAwareSerialization();
void SerializationChecklist();

// File 22
void PimplBasics();
void FastPimpl();
void SharedPimpl();
void WhenToUsePimpl();

// File 23
void AlignmentBasics();
void StackAlignment();
void HeapAlignment();
void SIMDAlignment();
void OverAlignedContainers();
void AlignmentTradeoffs();

// File 24
void FlexibleArrayBasics();
void UsingFlexibleArray();
void ModernAlternative();
void UseCases();
void DangersAndAlternatives();

// File 25
void EmptyBaseOptimization();
void StatelessFunctors();
void MultipleEmptyBases();
void WhenEboDoesntWork();
void ModernAlternative();