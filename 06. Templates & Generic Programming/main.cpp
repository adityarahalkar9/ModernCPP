import std;
#include "templates_header.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << " ModernCPP - 06. Templates Complete" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;

    // --- Function Templates (01) ---
    // BasicFunctionTemplate();
    // MultipleTypeParameters();
    // ReturnTypeDeduction();
    // TemplateOverloading();
    // HeaderDiscussion();
    // ExplicitInstantiation();

    // --- Class Templates (02) ---
    // BasicClassTemplate();
    // MultipleParameters();
    // MemberFunctionDefinition();
    // NestedTypes();
    // DefaultOperations();

    // --- Template Parameters (03) ---
    // TypeParameters();
    // MultipleTypeParams();
    // TemplateTemplateParams();
    // ParameterPacks();
    // AllParameterTypes();

    // --- Non-Type Parameters (04) ---
    // BasicNonType();
    // NonTypeArray();
    // CompileTimeComputation();
    // AutoNonType();
    // EnumNonType();
    // Cxx20NonType();

    // --- Template Template Parameters (05) ---
    // TemplateTemplateBasics();
    // CustomAllocatorTemplate();
    // PolicyBasedDesign();
    // Cxx17Improvements();

    // --- Default Arguments (06) ---
    // DefaultArguments();
    // MultipleDefaults();
    // DependentDefaults();
    // FunctionTemplateDefaults();
    // ConditionalDefaults();

    // --- Specialization (07) ---
    // FullSpecialization();
    // FunctionSpecialization();
    // SpecializationForSemantics();
    // SpecializationRules();

    // --- Partial Specialization (08) ---
    // PartialSpecializationBasics();
    // ContainerSpecialization();
    // TemplateTemplateSpecialization();
    // DetectionIdiom();

    // --- Variable Templates (09) ---
    // BasicVariableTemplate();
    // SpecializedVariables();
    // ComputedVariables();
    // VariableTemplatesForTraits();
    // MultipleParameterVariables();
    // PracticalVariables();

    // --- Deduction (10) ---
    // BasicDeduction();
    // ContainerDeduction();
    // DeductionFailures();
    // AutoDeduction();
    // InitListDeduction();
    // DeductionRules();

        // --- Deduction Guides (11) ---
    // CTADBasics();
    // WhyGuidesNeeded();
    // PracticalGuides();
    // StandardLibraryCTAD();
    // ImplicitGuides();

    // --- SFINAE (12) ---
    // BasicSFINAE();
    // ReturnTypeSFINAE();
    // ParameterSFINAE();
    // DetectingMembers();
    // ModernSFINAE();
    // SFINAELimitations();

    // --- enable_if (13) ---
    // BasicEnableIf();
    // EnableIfReturnType();
    // ConditionalClass();
    // MultipleConditions();
    // UsingAliasVersion();

    // --- type_traits (14) ---
    // PrimaryTypeCategories();
    // CompositeTypeCategories();
    // TypeProperties();
    // TypeFeatures();
    // TypeRelationships();
    // TypeTransformations();
    // CustomTypeTraits();
    // UsingTypeTraitsInTemplates();

    // --- Template Metaprogramming (15) ---
    // CompileTimeIfElse();
    // TypeLists();
    // IntegerSequences();
    // CompileTimeArithmetic();
    // TemplateRecursion();
    // CompileTimePower();
    // ModernConstexpr();

    // --- Compile-Time Computation (16) ---
    // CompileTimeArrays();
    // CompileTimeStrings();
    // CompileTimePrimes();
    // CompileTimeMath();
    // CompileTimeSorting();

    // --- Variadic Templates (17) ---
    // BasicVariadic();
    // RecursiveProcessing();
    // ModernVariadic();
    // VariadicSum();
    // VariadicClass();
    // VariadicForwarding();

    // --- Fold Expressions (18) ---
    // BasicFoldExpressions();
    // BinaryFoldExpressions();
    // DifferentOperators();
    // FoldWithComma();
    // FoldForPrinting();
    // FoldWithContainers();
    // EmptyPackHandling();

    // --- Template Recursion (19) ---
    // BasicRecursion();
    // PowerRecursion();
    // TypeRecursion();
    // SequenceGeneration();
    // StringRecursion();
    // RecursionLimitations();

    // --- CRTP (20) ---
    // BasicCRTP();
    // StaticPolymorphism();
    // CRTPMixins();
    // CRTPPerformance();

    // --- Concepts Basics (21) ---
    // BasicConcepts();
    // ConceptsWithRequires();
    // MultipleRequirements();
    // CombiningConcepts();
    // ConceptAsBoolean();

    // --- Standard Concepts (22) ---
    // EqualityConcepts();
    // OrderingConcepts();
    // ArithmeticConcepts();
    // CallableConcepts();
    // ConvertibleConcepts();
    // RangeConcepts();
    // StandardConceptsList();

    // --- Custom Concepts (23) ---
    // CustomContainerConcept();
    // CustomPrintableConcept();
    // ComplexConcept();
    // ArithmeticConcept();
    // UsingCustomConcepts();

    // --- requires Clause (24) ---
    // BasicRequiresClause();
    // MultipleConditionsRequires();
    // RequiresExpression();
    // CompoundRequirements();
    // RequiresInClass();

    // --- Constrained Overloading (25) ---
    // ConceptOverloading();
    // ConceptPriority();
    // ConceptsWithAuto();
    // ConceptDisambiguation();

    // --- Template Instantiation (26) ---
    // ImplicitInstantiation();
    // ExplicitInstantiationDemo();
    // SelectiveInstantiation();
    // TwoPhaseLookup();
    // CodeBloatConsiderations();

    // --- Dependent Names (27) ---
    // DependentTypeNames();
    // DependentTemplateNames();
    // DependentBaseClass();
    // DependentNamePitfalls();

    // --- Template Friends (28) ---
    // FriendFunctions();
    // FriendClasses();
    // SpecificInstantiation();
    // FriendOperators();
    // HiddenFriends();

    // --- Expression Templates (29) ---
    // NaiveApproach();
    // ExpressionTemplateApproach();
    // PerformanceComparison();

    // --- Policy Design (30) ---
    // PolicyBasedDesign();
    // TemplateTemplatePolicy();
    // PolicyBenefits();

    // --- Tag Dispatch (31) ---
    // IteratorTagDispatch();
    // NumericTagDispatch();
    // ContainerTagDispatch();
    // AlgorithmTagDispatch();

    // --- Type Erasure (32) ---
    // BasicTypeErasure();
    // AdvancedTypeErasure();
    // StdFunctionTypeErasure();
    // TypeErasureVsVirtual();

    // --- Template Aliases (33) ---
    // BasicTemplateAliases();
    // AliasesWithDefaults();
    // ComplexTypeAliases();
    // AliasVsTypedef();
    // PracticalPatterns();

    // --- Abbreviated Templates (34) ---
    // BasicAbbreviated();
    // MultipleAutoParams();
    // AutoWithConcepts();
    // AutoReturnType();
    // GenericLambdas();

    // --- Pitfalls (35) ---
    // TypenamePitfall();
    // TemplatePitfall();
    // DependentBasePitfall();
    // CodeBloatPitfall();
    // SpecializationPitfall();
    // ConversionPitfall();

    // --- Best Practices (36) ---
    // BestPracticesList();
    // DesignGuidelines();
    // ErrorHandlingBestPractices();
    // PerformanceBestPractices();
    // ModernGuidelines();

    // --- Compile-Time Strings (37) ---
    // BasicCompileTimeStrings();
    // CompileTimeStringComparison();
    // CompileTimeConcat();
    // CompileTimeHashing();
    // StringTemplates();

    // --- Heterogeneous Containers (38) ---
    // TupleBasics();
    // TupleIteration();
    // VariantBasics();
    // VariantVisitation();
    // CustomHeteroContainer();
    // CompileTimeTypeLists();


    return 0;
}