import std;
#include "smart_pointers_header.h"

// Simple scope guard
class ScopeGuard {
public:
    explicit ScopeGuard(std::function<void()> exit_action)
        : exit_action_(std::move(exit_action)) {
        std::cout << "  [ENTER] Scope guard created" << std::endl;
    }

    ~ScopeGuard() {
        if(active_) {
            std::cout << "  [EXIT] Running exit action" << std::endl;
            exit_action_();
        }
    }

    // Non-copyable, non-movable (usually)
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    void dismiss() {
        active_ = false;
        std::cout << "  [DISMISS] Exit action will NOT run" << std::endl;
    }

private:
    std::function<void()> exit_action_;
    bool active_ = true;
};

void BasicScopeGuard() {
    std::cout << "=== Basic Scope Guard ===" << std::endl;

    std::cout << "Starting operation:" << std::endl;
    {
        ScopeGuard guard([]() {
            std::cout << "    Cleanup: releasing resources" << std::endl;
                         });

        std::cout << "  Doing work..." << std::endl;
    }
    std::cout << "Operation complete" << std::endl;
}

void ScopeGuardWithDismiss() {
    std::cout << "\n=== Scope Guard with Dismiss ===" << std::endl;

    std::cout << "Successful operation (dismiss guard):" << std::endl;
    {
        ScopeGuard guard([]() {
            std::cout << "    ERROR CLEANUP: rolling back" << std::endl;
                         });

        std::cout << "  Work succeeded, dismissing guard" << std::endl;
        guard.dismiss();  // Don't run cleanup
    }

    std::cout << "\nFailed operation (guard runs):" << std::endl;
    {
        ScopeGuard guard([]() {
            std::cout << "    ERROR CLEANUP: rolling back" << std::endl;
                         });

        std::cout << "  Work failed, guard will run" << std::endl;
        // guard not dismissed, cleanup runs
    }
}

// Scope guard for transactions
class Transaction2 {
public:
    Transaction2() {
        std::cout << "  Transaction started" << std::endl;
    }

    ~Transaction2() {
        if(!committed_) {
            std::cout << "  [ROLLBACK] Transaction rolled back" << std::endl;
        }
        else {
            std::cout << "  [COMMIT] Transaction committed" << std::endl;
        }
    }

    void commit() {
        committed_ = true;
        std::cout << "  Marking for commit" << std::endl;
    }

private:
    bool committed_ = false;
};

void TransactionScopeGuard() {
    std::cout << "\n=== Transaction Scope Guard ===" << std::endl;

    std::cout << "Successful transaction:" << std::endl;
    {
        Transaction2 txn;
        std::cout << "  Doing transaction work..." << std::endl;
        txn.commit();
    }

    std::cout << "\nFailed transaction:" << std::endl;
    {
        Transaction2 txn;
        std::cout << "  Work failed, not committing" << std::endl;
        // No commit → rollback in destructor
    }
}

// Generic scope exit
enum class ScopeExitAction {
    OnSuccess,  // Run only if no exception
    OnFailure,  // Run only if exception
    Always      // Run unconditionally
};

template <ScopeExitAction Action>
class ConditionalScopeGuard {
public:
    explicit ConditionalScopeGuard(std::function<void()> fn)
        : fn_(std::move(fn)) {
    }

    ~ConditionalScopeGuard() {
        bool has_exception = std::uncaught_exceptions() > 0;

        if constexpr(Action == ScopeExitAction::OnSuccess) {
            if(!has_exception) fn_();
        }
        else if constexpr(Action == ScopeExitAction::OnFailure) {
            if(has_exception) fn_();
        }
        else {
            fn_();
        }
    }

private:
    std::function<void()> fn_;
};

void ConditionalScopeGuards() {
    std::cout << "\n=== Conditional Scope Guards ===" << std::endl;

    std::cout << "With exception:" << std::endl;
    try {
        ConditionalScopeGuard<ScopeExitAction::OnSuccess> on_success([]() {
            std::cout << "    SUCCESS: runs (no exception)" << std::endl;
                                                                     });

        ConditionalScopeGuard<ScopeExitAction::OnFailure> on_failure([]() {
            std::cout << "    FAILURE: runs (exception occurred)" << std::endl;
                                                                     });

        ConditionalScopeGuard<ScopeExitAction::Always> always([]() {
            std::cout << "    ALWAYS: runs regardless" << std::endl;
                                                              });

        throw std::runtime_error("test");
    }
    catch(...) {
        std::cout << "  Exception handled" << std::endl;
    }

    std::cout << "\nWithout exception:" << std::endl;
    {
        ConditionalScopeGuard<ScopeExitAction::OnSuccess> on_success([]() {
            std::cout << "    SUCCESS: runs (no exception)" << std::endl;
                                                                     });

        ConditionalScopeGuard<ScopeExitAction::OnFailure> on_failure([]() {
            std::cout << "    FAILURE: runs (exception occurred)" << std::endl;
                                                                     });

        ConditionalScopeGuard<ScopeExitAction::Always> always([]() {
            std::cout << "    ALWAYS: runs regardless" << std::endl;
                                                              });
    }
}

// Defer-like pattern (Go style)
#define DEFER_CONCAT_IMPL(a, b) a##b
#define DEFER_CONCAT(a, b) DEFER_CONCAT_IMPL(a, b)
#define DEFER(code) \
    ScopeGuard DEFER_CONCAT(_scope_guard_, __LINE__)([&]() { code; })

void DeferPattern() {
    std::cout << "\n=== Defer Pattern (Go-style) ===" << std::endl;

    std::cout << "Using DEFER macro:" << std::endl;
    {
        DEFER(std::cout << "    Deferred action 3" << std::endl);
        DEFER(std::cout << "    Deferred action 2" << std::endl);
        DEFER(std::cout << "    Deferred action 1" << std::endl);

        std::cout << "  Main work..." << std::endl;
    }
    // Deferred actions run in REVERSE order
}