#pragma once

void HelloWorld();
void KeywordDemo();
void VariableDemo();
void DataTypes();
int Functions();
int Operators();
int Conditions();
int ControlFlow();
int Size_t_size_of();
int Arrays();
int StlArray();

// 12.type_deduction.cpp
void AutoBasic();
void AutoWithModifiers();
void AutoReturnTypes();
void DecltypeBasic();
void DecltypeExpressions();
void DecltypeAuto();
void AbbreviatedFunctionTemplates();
void AutoWithStructuredBindings();
// void DecayCopy();

// 13.auto_&_decltype.cpp
void AutoDeductionRules();
void AutoWithContainers();
void AutoInLambdas();
void TrailingReturnType();
void DecltypeInTypeTraits();
void DecltypeInTemplates();
void DecltypeAutoForwarding();
void AutoWithConcepts();
void AutoAsTemplateParameter();

// 14.string_basics.cpp
void StringBasics();
void StringElementAccess();
void StringModification();
void StringSearch();
void StringComparison();
void StringViewBasics();
void StringConversions();
void RawStringLiterals();
void Char8_t();

// 15.enumerations.cpp
void UnscopedEnum();
void EnumWithBaseType();
void ScopedEnum();
void ScopedEnumWithBase();
void EnumFlags();
void ToUnderlying();
void IsScopedEnum();
void EnumInSwitch();
void ForwardDeclaredEnum();
void EnumIteration();

// 16.type_casting.cpp
void StaticCastDemo();
void DynamicCastDemo();
void ConstCastDemo();
void ReinterpretCastDemo();
void CStyleCastDemo();
void ImplicitVsExplicit();
void SmartPointerCasts();

// 17.input_output.cpp
void BasicIO();
void FormattedOutput();
void InputHandling();
void StringStreams();
void FileIO();
void FormatDemo();
void PrintDemo();
void StreamStates();
void CustomOutputOperator();

// 18.compound_types.cpp
void PairBasics();
void TupleBasics();
void StructuredBindings();
void OptionalBasics();
void VariantBasics();
void AnyBasics();
void ExpectedBasics();

// 19.initialization.cpp
void InitializationSyntaxes();
void AggregateInitialization();
void DesignatedInitializers();
void ContainerInitialization();
void ValueVsDefaultInit();
void CopyVsDirectListInit();
void ParenthesizedInit();
void InitializationOrder();

// 20.scope_&_lifetime.cpp
void ScopeBasics();
void VariableShadowing();
void ObjectLifetime();
void StaticLocalVariables();
void RAIIDemo();
void LifetimeExtension();
void DanglingReferences();
void DynamicLifetime();
void ThreadStorageDemo();

// 21.preprocessor.cpp
void BasicMacros();
void ConditionalCompilation();
void PredefinedMacros();
void TokenManipulation();
void VariadicMacros();
void FeatureTestingMacros();
void WarningDirective();
void CompileTimeAssertions();
void LineContinuation();

// 22.namespaces.cpp
void BasicNamespace();
void UsingDeclarations();
void NamespaceAliases();
void AnonymousNamespace();
void InlineNamespace();
void NestedNamespaceSyntax();
void ADLDemo();
void NameCollisionResolution();
void StdNamespaceBestPractices();

// 23.structured_bindings.cpp
void PairBindings();
void TupleBindings();
void StructBindings();
void ArrayBindings();
void MapBindings();
void RangeForBindings();
void CustomTypeBindings();
void IfWithBindings();
void LambdaCaptureBindings();
void InitStatementBindings();

//	24.designated_initializers.cpp
void BasicDesignatedInit();
void DesignatedInitRules();
void ArrayDesignatedInit();
void BeforeAndAfterComparison();
void PracticalExamples();
void DesignatedInitWithInheritance();

// 25.three_way_comparison.cpp
void BasicSpaceship();
void ComparisonCategories();
void CustomComparison();
void CompareThreeWay();
void ContainerComparison();
void EqualityAndOrdering();
void EqualityOnly();

// 26.char8_t.cpp
void Char8_tBasics();
void U8String();
void UTF8Handling();
void U8StringInView();
void UnicodeEscapes();
void CharacterTypesComparison();
void MigrationNotes();

// 27.attributes.cpp
void NodiscardAttribute();
void MaybeUnusedAttribute();
void DeprecatedAttribute();
void FallthroughAttribute();
void LikelyUnlikelyAttributes();
void NoUniqueAddressAttribute();
void NoReturnAttribute();
void AttributeSyntaxes();
void AssumeAttribute();
void AttributesOnLambdas();
void MultipleAttributes();