#pragma once

// File 01 - Function templates
void BasicFunctionTemplate();
void MultipleTypeParameters();
void ReturnTypeDeduction();
void TemplateOverloading();
void HeaderDiscussion();
void ExplicitInstantiation();

// File 02 - Class templates
void BasicClassTemplate();
void MultipleParameters();
void MemberFunctionDefinition();
void NestedTypes();
void DefaultOperations();

// File 03 - Template parameters
void TypeParameters();
void MultipleTypeParams();
void TemplateTemplateParams();
void ParameterPacks();
void AllParameterTypes();

// File 04 - Non-type parameters
void BasicNonType();
void NonTypeArray();
void CompileTimeComputation();
void AutoNonType();
void EnumNonType();
void Cxx20NonType();

// File 05 - Template template parameters
void TemplateTemplateBasics();
void CustomAllocatorTemplate();
void PolicyBasedDesign();
void Cxx17Improvements();

// File 06 - Default arguments
void DefaultArguments();
void MultipleDefaults();
void DependentDefaults();
void FunctionTemplateDefaults();
void ConditionalDefaults();

// File 07 - Specialization
void FullSpecialization();
void FunctionSpecialization();
void SpecializationForSemantics();
void SpecializationRules();

// File 08 - Partial specialization
void PartialSpecializationBasics();
void ContainerSpecialization();
void TemplateTemplateSpecialization();
void DetectionIdiom();

// File 09 - Variable templates
void BasicVariableTemplate();
void SpecializedVariables();
void ComputedVariables();
void VariableTemplatesForTraits();
void MultipleParameterVariables();
void PracticalVariables();

// File 10 - Deduction
void BasicDeduction();
void ContainerDeduction();
void DeductionFailures();
void AutoDeduction();
void InitListDeduction();
void DeductionRules();

// File 11 - Deduction guides
void CTADBasics();
void WhyGuidesNeeded();
void PracticalGuides();
void StandardLibraryCTAD();
void ImplicitGuides();

// File 12 - SFINAE
void BasicSFINAE();
void ReturnTypeSFINAE();
void ParameterSFINAE();
void DetectingMembers();
void ModernSFINAE();
void SFINAELimitations();

// File 13 - enable_if
void BasicEnableIf();
void EnableIfReturnType();
void ConditionalClass();
void MultipleConditions();
void UsingAliasVersion();

// File 14 - type_traits
void PrimaryTypeCategories();
void CompositeTypeCategories();
void TypeProperties();
void TypeFeatures();
void TypeRelationships();
void TypeTransformations();
void CustomTypeTraits();
void UsingTypeTraitsInTemplates();

// File 15 - template_metaprogramming
void CompileTimeIfElse();
void TypeLists();
void IntegerSequences();
void CompileTimeArithmetic();
void TemplateRecursion();
void CompileTimePower();
void ModernConstexpr();

// File 16 - compile_time_computation
void CompileTimeArrays();
void CompileTimeStrings();
void CompileTimePrimes();
void CompileTimeMath();
void CompileTimeSorting();

// File 17 - variadic_templates
void BasicVariadic();
void RecursiveProcessing();
void ModernVariadic();
void VariadicSum();
void VariadicClass();
void VariadicForwarding();

// File 18 - fold_expressions
void BasicFoldExpressions();
void BinaryFoldExpressions();
void DifferentOperators();
void FoldWithComma();
void FoldForPrinting();
void FoldWithContainers();
void EmptyPackHandling();

// File 19 - template_recursion
void BasicRecursion();
void PowerRecursion();
void TypeRecursion();
void SequenceGeneration();
void StringRecursion();
void RecursionLimitations();

// File 20 - CRP
void BasicCRTP();
void StaticPolymorphism();
void CRTPMixins();
void CRTPPerformance();

// File 21 - concepts_basics
void BasicConcepts();
void ConceptsWithRequires();
void MultipleRequirements();
void CombiningConcepts();
void ConceptAsBoolean();

// File 22 - concepts_standard
void EqualityConcepts();
void OrderingConcepts();
void ArithmeticConcepts();
void CallableConcepts();
void ConvertibleConcepts();
void RangeConcepts();
void StandardConceptsList();

// File 23 - concepts_custom
void CustomContainerConcept();
void CustomPrintableConcept();
void ComplexConcept();
void ArithmeticConcept();
void UsingCustomConcepts();

// File 24 - requires_clause
void BasicRequiresClause();
void MultipleConditionsRequires();
void RequiresExpression();
void CompoundRequirements();
void RequiresInClass();

// File 25 - constrained_overloading
void ConceptOverloading();
void ConceptPriority();
void ConceptsWithAuto();
void ConceptDisambiguation();

// File 26 - Template Instantiation
void ImplicitInstantiation();
void ExplicitInstantiationDemo();
void SelectiveInstantiation();
void TwoPhaseLookup();
void CodeBloatConsiderations();

// File 27 - Dependent Names
void DependentTypeNames();
void DependentTemplateNames();
void DependentBaseClass();
void DependentNamePitfalls();

// File 28 - Template Friends
void FriendFunctions();
void FriendClasses();
void SpecificInstantiation();
void FriendOperators();
void HiddenFriends();

// File 29 - Expression Templates
void NaiveApproach();
void ExpressionTemplateApproach();
void PerformanceComparison();

// File 30 - Policy Design
void PolicyBasedDesign();
void TemplateTemplatePolicy();
void PolicyBenefits();

// File 31 - Tag Dispatch
void IteratorTagDispatch();
void NumericTagDispatch();
void ContainerTagDispatch();
void AlgorithmTagDispatch();

// File 32 - Type Erasure Templates
void BasicTypeErasure();
void AdvancedTypeErasure();
void StdFunctionTypeErasure();
void TypeErasureVsVirtual();

// File 33 - Template Aliases
void BasicTemplateAliases();
void AliasesWithDefaults();
void ComplexTypeAliases();
void AliasVsTypedef();
void PracticalPatterns();

// File 34 - Abbreviated Templates
void BasicAbbreviated();
void MultipleAutoParams();
void AutoWithConcepts();
void AutoReturnType();
void GenericLambdas();

// File 35 - Template Pitfalls
void TypenamePitfall();
void TemplatePitfall();
void DependentBasePitfall();
void CodeBloatPitfall();
void SpecializationPitfall();
void ConversionPitfall();

// File 36 - Best Practices
void BestPracticesList();
void DesignGuidelines();
void ErrorHandlingBestPractices();
void PerformanceBestPractices();
void ModernGuidelines();

// File 37 - Compile-Time Strings
void BasicCompileTimeStrings();
void CompileTimeStringComparison();
void CompileTimeConcat();
void CompileTimeHashing();
void StringTemplates();

// File 38 - Heterogeneous Containers
void TupleBasics();
void TupleIteration();
void VariantBasics();
void VariantVisitation();
void CustomHeteroContainer();
void CompileTimeTypeLists();