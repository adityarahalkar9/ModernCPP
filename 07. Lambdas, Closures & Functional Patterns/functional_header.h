#pragma once

// File 01 - Lambda Syntax
void LambdaAnatomy();
void TheClosureObject();
void LambdaVariations();
void ImmediatelyExplainedConcepts();

// File 02 - Lambda Captures
void NoCaptureDefault();
void ExplicitCapture();
void CaptureAllSyntax();
void CaptureRules();
void WhenCapturesMatter();

// File 03 - Capture by Value
void ValueCaptureSemantics();
void ValueCaptureCost();
void ValueCaptureLifetime();
void ValueCaptureConst();

// File 04 - Capture by Reference
void ReferenceCaptureBasics();
void ReferenceForAccumulation();
void TheDanglingDanger();
void ReferenceVsValueDecision();

// File 05 - Mutable Lambdas
void MutableBasics();
void StatefulCounter();
void UniqueIdGenerator();
void CachePattern();
void MutableGotchas();

// File 06 - Init Captures
void InitCaptureBasics();
void TheMoveRevolution();
void ComplexCaptures();
void PracticalPatterns();

// File 07 - Generic Lambdas
void GenericLambdaBasics();
void DifferentTypesPerParameter();
void GenericLambdasWithSTL();
void Cxx20Improvements();
void WhenToUseGeneric();

// File 08 - Lambda and this
void ThisCaptureBasics();
void TheDanglingThisProblem();
void CaptureStrategies();

// File 09 - Recursive Lambdas
void StdFunctionApproach();
void YCombinator();
void Cxx23DeducingThis();
void PracticalRecursion();

// File 10 - IIFE
void BasicIIFE();
void ComplexInitialization();
void StaticInitialization();
void ScopeIsolation();

// File 11 - std::function
void BasicStdFunction();
void StoringCallbacks();
void FunctionVsAuto();
void PerformanceConsiderations();

// File 12 - Closures
void WhatIsAClosure();
void ClosureFactory();
void StatefulClosures();
void ClosuresInPractice();

// File 13 - Higher-Order Functions
void TakingFunctions();
void ReturningFunctions();
void TheBigThree();
void ComposingOperations();

// File 14 - Function Composition
void BasicComposition();
void ChainedComposition();
void StringProcessingPipeline();
void DataAnalysisPipeline();

// File 15 - Currying
void CurryingBasics();
void PartialApplication();
void PracticalCurrying();
void CurryingVsPartial();

// File 16 - Monadic Patterns
void ThePyramidProblem();
void MonadicChaining();
void TransformVsAndThen();
void OrElsePattern();

// File 17 - Pattern Matching
void VariantBasics();
void PatternMatching();
void overloadedPattern();
void StateMachinePattern();

// File 18 - Functional Algorithms
void TransformExamples();
void FilterExamples();
void FindExamples();
void CheckExamples();
void FoldExamples();

// File 19 - Lazy Evaluation
void DeferredEvaluation();
void MemoizationPattern();
void InfiniteSequences();
void ShortCircuitEvaluation();

// File 20 - Immutability
void ConstCorrectness();
void PureFunctions();
void ValueSemantics();
void ImmutabilityTradeoffs();

// File 21 - Pipelines
void BasicPipeline();
void PipelineBuilder();
void ErrorHandlingPipeline();

// File 22 - Pitfalls
void Pitfall1_DanglingReference();
void Pitfall2_CapturedThis();
void Pitfall3_AccidentalCopy();
void Pitfall4_MutableSurprise();
void Pitfall5_CaptureInLoops();

// File 23 - Performance
void SizeOverhead();
void FunctionCallOverhead();
void OptimizationTips();

// File 24 - C++23 Features
void AttributeOnLambdas();
void StaticOperator();
void DeducingThisInLambdas();
void OtherImprovements();

// File 25 - Best Practices
void BestPracticesSummary();
void WhenToUseWhat();
void CommonMistakes();