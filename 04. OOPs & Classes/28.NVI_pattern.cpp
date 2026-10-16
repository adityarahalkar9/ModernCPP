import std;
#include "class_header.h"

// Non-Virtual Interface (NVI) Pattern
class GameCharacter {
public:
    // Public non-virtual interface
    // Defines the ALGORITHM structure
    void attack(GameCharacter& target) {
        // Pre-processing (invariant)
        std::cout << "  [Pre-attack validation]" << std::endl;

        // The actual work (customizable)
        do_attack(target);

        // Post-processing (invariant)
        std::cout << "  [Post-attack bookkeeping]" << std::endl;
    }

    void defend() {
        std::cout << "  [Pre-defend setup]" << std::endl;
        do_defend();
        std::cout << "  [Post-defend cleanup]" << std::endl;
    }

    virtual ~GameCharacter() = default;

protected:
    // Virtual implementation details
    // Derived classes customize these
    virtual void do_attack(GameCharacter& target) {
        std::cout << "  Default attack implementation" << std::endl;
    }

    virtual void do_defend() {
        std::cout << "  Default defend implementation" << std::endl;
    }
};

// Custom character types
class Warrior : public GameCharacter {
protected:
    void do_attack(GameCharacter& target) override {
        std::cout << "  Warrior swings sword!" << std::endl;
    }

    void do_defend() override {
        std::cout << "  Warrior raises shield!" << std::endl;
    }
};

class Mage : public GameCharacter {
protected:
    void do_attack(GameCharacter& target) override {
        std::cout << "  Mage casts fireball!" << std::endl;
    }

    void do_defend() override {
        std::cout << "  Mage creates magic barrier!" << std::endl;
    }
};

void NVIBasics() {
    std::cout << "=== Non-Virtual Interface Pattern ===" << std::endl;

    std::cout << "NVI Pattern:" << std::endl;
    std::cout << "  Public: non-virtual interface" << std::endl;
    std::cout << "  Protected/Private: virtual implementation" << std::endl;
    std::cout << "  Base controls the algorithm" << std::endl;

    Warrior warrior;
    Mage mage;

    std::cout << "\nWarrior attacks:" << std::endl;
    warrior.attack(mage);

    std::cout << "\nMage defends:" << std::endl;
    mage.defend();
}

// NVI with validation
class BankAccount {
private:
    double balance = 0;

public:
    // NVI: withdraw enforces invariants
    bool withdraw(double amount) {
        std::cout << "  [Validating withdrawal]" << std::endl;

        if(amount <= 0) {
            std::cout << "  [Invalid amount]" << std::endl;
            return false;
        }

        if(amount > balance) {
            std::cout << "  [Insufficient funds]" << std::endl;
            return false;
        }

        // Let derived class do the actual work
        bool success = do_withdraw(amount);

        if(success) {
            balance -= amount;
            std::cout << "  [Balance updated: " << balance << "]" << std::endl;
        }

        return success;
    }

    double get_balance() const { return balance; }

    void deposit(double amount) {
        if(do_deposit(amount)) {
            balance += amount;
        }
    }

    virtual ~BankAccount() = default;

protected:
    // Virtual hooks for customization
    virtual bool do_withdraw(double amount) {
        std::cout << "  Standard withdrawal of " << amount << std::endl;
        return true;  // Base implementation always succeeds
    }

    virtual bool do_deposit(double amount) {
        std::cout << "  Standard deposit of " << amount << std::endl;
        return amount > 0;
    }
};

// Special account with fees
class FeeAccount : public BankAccount {
protected:
    bool do_withdraw(double amount) override {
        double fee = amount * 0.01;  // 1% fee
        std::cout << "  Fee account: " << amount << " + fee " << fee << std::endl;
        return true;
    }
};

void NVIWithValidation() {
    std::cout << "\n=== NVI with Validation ===" << std::endl;

    FeeAccount account;
    account.deposit(1000);
    std::cout << "Balance: " << account.get_balance() << std::endl;

    std::cout << "\nWithdrawing:" << std::endl;
    account.withdraw(100);
    std::cout << "Balance: " << account.get_balance() << std::endl;

    std::cout << "\nInvalid withdrawal:" << std::endl;
    account.withdraw(-50);
    account.withdraw(5000);
}

// NVI with template method
class DataProcessor {
public:
    // Template method pattern via NVI
    void process(const std::string& data) {
        std::cout << "  === Processing pipeline ===" << std::endl;

        auto validated = validate(data);
        if(!validated.empty()) {
            auto transformed = transform(validated);
            auto result = compute(transformed);
            output(result);
        }

        std::cout << "  === Pipeline complete ===" << std::endl;
    }

    virtual ~DataProcessor() = default;

private:
    // Each step can be customized
    virtual std::string validate(const std::string& input) {
        std::cout << "  Step 1: Validating" << std::endl;
        return input.empty() ? "" : input;
    }

    virtual std::string transform(const std::string& input) {
        std::cout << "  Step 2: Transforming" << std::endl;
        return input;
    }

    virtual std::string compute(const std::string& input) {
        std::cout << "  Step 3: Computing" << std::endl;
        return "result: " + input;
    }

    virtual void output(const std::string& result) {
        std::cout << "  Step 4: " << result << std::endl;
    }
};

// Custom processor
class UpperCaseProcessor : public DataProcessor {
private:
    std::string transform(const std::string& input) override {
        std::string upper = input;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        return upper;
    }
};

void NVITemplateMethod() {
    std::cout << "\n=== NVI Template Method ===" << std::endl;

    std::cout << "NVI implements Template Method:" << std::endl;
    std::cout << "  Algorithm structure is fixed" << std::endl;
    std::cout << "  Steps are customizable" << std::endl;

    std::cout << "\nDefault processor:" << std::endl;
    DataProcessor default_processor;
    default_processor.process("hello world");

    std::cout << "\nCustom processor:" << std::endl;
    UpperCaseProcessor upper_processor;
    upper_processor.process("hello world");
}

// Benefits of NVI
void NVIBenefits() {
    std::cout << "\n=== NVI Benefits ===" << std::endl;

    std::cout << "1. Invariant enforcement:" << std::endl;
    std::cout << "   Base class controls pre/post conditions" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Algorithm structure:" << std::endl;
    std::cout << "   Steps are in fixed order" << std::endl;
    std::cout << "   Derived can't break the flow" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Future evolution:" << std::endl;
    std::cout << "   Can add logging/metrics without derived knowing" << std::endl;
    std::cout << std::endl;

    std::cout << "4. Clearer contract:" << std::endl;
    std::cout << "   Public interface is stable" << std::endl;
    std::cout << "   Implementation details are hidden" << std::endl;
}