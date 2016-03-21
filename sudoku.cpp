#include <map>

namespace sudoku {
    template <class CoordSet, class ValueSet>
    class SolutionIdea {
    public:
        std::map<typename CoordSet::value_type, ValueSet> cells;
        bool possible(void) const {
            for (auto el : cells) {
                if (el->second.empty()) {
                    return false;
                }
            }
            return true;
        }
    };

    class Constraint {
    public:
        virtual bool check(void) const = 0;
    };

    template <class CoordSet, class ValueSet>
    class UniqueConstraint : public Constraint {
    public:
        SolutionIdea<CoordSet, ValueSet> idea;
        CoordSet area;
        bool check(void) const override {
            ValueSet s{};
            for (auto el : area) {
                auto cell_set = idea.cells[el];
                if (cell_set.size() > 1) {
                    continue;
                }
                if (!s.insert(*cell_set.begin()).second) {
                    return false;
                }
            }
            return true;
        }
    };

    template <class CoordSet, class ValueSet>
    class SumConstraint : public Constraint {
    public:
        SolutionIdea<CoordSet, ValueSet> idea;
        CoordSet area;
        typename ValueSet::value_type desired_sum;
        bool check(void) const override {
            typename ValueSet::value_type sum{};
            for (auto el : area) {
                auto cell_set = idea.cells[el];
                if (cell_set.size() > 1) {
                    return true;
                }
                sum = sum + *cell_set.begin();
            }
            return sum == desired_sum;
        }
    };
}
