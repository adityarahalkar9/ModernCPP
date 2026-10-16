import std;
#include "smart_pointers_header.h"

class Item {
public:
    explicit Item(std::string name, int value = 0)
        : name_(std::move(name)), value_(value) {
        std::cout << "  Item('" << name_ << "') created" << std::endl;
    }

    ~Item() {
        std::cout << "  Item('" << name_ << "') destroyed" << std::endl;
    }

    const std::string& name() const { return name_; }
    int value() const { return value_; }

    bool operator<(const Item& other) const {
        return name_ < other.name_;
    }

private:
    std::string name_;
    int value_;
};

void UniquePtrInVector() {
    std::cout << "=== unique_ptr in vector ===" << std::endl;

    std::vector<std::unique_ptr<Item>> items;

    std::cout << "Adding items:" << std::endl;
    items.push_back(std::make_unique<Item>("first", 10));
    items.push_back(std::make_unique<Item>("second", 20));
    items.emplace_back("third", 30);

    std::cout << "\nIterating:" << std::endl;
    for(const auto& item : items) {
        std::cout << "  " << item->name() << ": " << item->value() << std::endl;
    }

    std::cout << "\nSorting by name:" << std::endl;
    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b) {
                  return a->name() > b->name();  // Reverse order
              });

    for(const auto& item : items) {
        std::cout << "  " << item->name() << std::endl;
    }
}

void SharedPtrInVector() {
    std::cout << "\n=== shared_ptr in vector ===" << std::endl;

    std::vector<std::shared_ptr<Item>> shared_items;

    auto item1 = std::make_shared<Item>("shared1", 100);
    shared_items.push_back(item1);
    shared_items.push_back(item1);  // Same object twice!
    shared_items.push_back(std::make_shared<Item>("shared2", 200));

    std::cout << "item1 use_count: " << item1.use_count() << std::endl;

    std::cout << "\nContents:" << std::endl;
    for(const auto& item : shared_items) {
        std::cout << "  " << item->name() << " (count: " << item.use_count() << ")" << std::endl;
    }
}

void SmartPointersInMaps() {
    std::cout << "\n=== Smart Pointers in Maps ===" << std::endl;

    std::map<std::string, std::unique_ptr<Item>> item_map;

    std::cout << "Inserting:" << std::endl;
    item_map["first"] = std::make_unique<Item>("first", 1);
    item_map.emplace("second", std::make_unique<Item>("second", 2));

    std::cout << "\nIterating map:" << std::endl;
    for(const auto& [key, value] : item_map) {
        std::cout << "  " << key << " -> " << value->name()
            << " (" << value->value() << ")" << std::endl;
    }

    std::cout << "\nLookup:" << std::endl;
    if(auto it = item_map.find("first"); it != item_map.end()) {
        std::cout << "  Found: " << it->second->name() << std::endl;
    }
}

void SmartPointersInSets() {
    std::cout << "\n=== Smart Pointers in Sets ===" << std::endl;

    // Set of shared_ptr (compares by pointer value)
    std::set<std::shared_ptr<Item>> item_set;

    auto item1 = std::make_shared<Item>("set1");
    auto item2 = std::make_shared<Item>("set2");
    auto item1_again = item1;  // Same object

    item_set.insert(item1);
    item_set.insert(item2);
    item_set.insert(item1_again);  // Won't insert (same pointer)

    std::cout << "Set size: " << item_set.size() << " (not 3)" << std::endl;

    // Custom comparator comparing pointed-to objects
    struct ItemComparator {
        bool operator()(const std::shared_ptr<Item>& a,
                        const std::shared_ptr<Item>& b) const {
            return *a < *b;
        }
    };

    std::set<std::shared_ptr<Item>, ItemComparator> sorted_set;
    sorted_set.insert(std::make_shared<Item>("zebra"));
    sorted_set.insert(std::make_shared<Item>("apple"));

    std::cout << "\nSorted by name:" << std::endl;
    for(const auto& item : sorted_set) {
        std::cout << "  " << item->name() << std::endl;
    }
}

void MovingSmartPointers() {
    std::cout << "\n=== Moving Smart Pointers ===" << std::endl;

    std::vector<std::unique_ptr<Item>> source;
    source.push_back(std::make_unique<Item>("movable1"));
    source.push_back(std::make_unique<Item>("movable2"));

    std::cout << "Source size before move: " << source.size() << std::endl;

    // Move entire vector
    std::vector<std::unique_ptr<Item>> destination = std::move(source);

    std::cout << "Source size after move: " << source.size() << std::endl;
    std::cout << "Destination size: " << destination.size() << std::endl;

    // Move individual elements
    std::vector<std::unique_ptr<Item>> another;
    another.push_back(std::move(destination[0]));

    std::cout << "After moving first element:" << std::endl;
    std::cout << "  destination[0] is null: " << (destination[0] == nullptr) << std::endl;
    std::cout << "  another[0] name: " << another[0]->name() << std::endl;
}

void ContainerOfSmartPointersPatterns() {
    std::cout << "\n=== Patterns ===" << std::endl;

    std::cout << "Common patterns:" << std::endl;
    std::cout << std::endl;

    std::cout << "1. Factory returns container of unique_ptr:" << std::endl;
    std::cout << "   auto create_all() {" << std::endl;
    std::cout << "       std::vector<std::unique_ptr<T>> result;" << std::endl;
    std::cout << "       result.push_back(std::make_unique<T>());" << std::endl;
    std::cout << "       return result;" << std::endl;
    std::cout << "   }" << std::endl;
    std::cout << std::endl;

    std::cout << "2. Store unique_ptr, expose raw/reference:" << std::endl;
    std::cout << "   class Owner {" << std::endl;
    std::cout << "       std::unique_ptr<T> owned_;" << std::endl;
    std::cout << "   public:" << std::endl;
    std::cout << "       T& get() { return *owned_; }  // Borrow, not own" << std::endl;
    std::cout << "   };" << std::endl;
    std::cout << std::endl;

    std::cout << "3. Observer pattern with weak_ptr:" << std::endl;
    std::cout << "   std::vector<std::weak_ptr<Observer>> observers;" << std::endl;
}