#include <map>

namespace sudoku {
    template <class CoordSet, class ValueSet>
    class SolutionIdea {
    public:
        // possible values for each cell
        std::map<typename CoordSet::value_type, ValueSet> cells;
        bool possible(void) const {
            // checks whether all cells have any possibilities left
            for (auto el : cells) {
                if (el->second.empty()) {
                    return false;
                }
            }
            return true;
        }
    };

    template <class CoordSet, class ValueSet>
    class Constraint {
    public:
        virtual bool check(const SolutionIdea<CoordSet, ValueSet>&) const = 0;
    };

    template <class CoordSet, class ValueSet>
    class UniqueConstraint : public Constraint<CoordSet, ValueSet> {
    public:
        CoordSet area;
        bool check(const SolutionIdea<CoordSet, ValueSet> &idea) const override {
            ValueSet s{};
            for (auto el : area) {
                auto cell_set = idea.cells[el];
                if (cell_set.size() > 1) {
                    // skip processing this cell
                    // we can still detect other collisions
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
    class SumConstraint : public Constraint<CoordSet, ValueSet> {
    public:
        CoordSet area;
        typename ValueSet::value_type desired_sum;
        bool check(const SolutionIdea<CoordSet, ValueSet> &idea) const override {
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

    template <class CoordSet, class ValueSet>
    class Strategy {
    public:
        Constraint<CoordSet, ValueSet> *constraint;
        virtual bool advance(SolutionIdea<CoordSet, ValueSet> &) const = 0;
    };

    template <class CoordSet, class ValueSet>
    class SumStrategy : public Strategy<CoordSet, ValueSet> {
    public:
        bool advance(SolutionIdea<CoordSet, ValueSet> &idea) const override {
            auto con = static_cast<SumConstraint<CoordSet, ValueSet>*>(this->constraint);
            typename ValueSet::value_type sum{};
            typename CoordSet::value_type missing;
            bool found_missing = false;
            for (auto el : con->area) {
                auto &cell_set = idea.cells[el];
                if (cell_set.size() > 1) {
                    if (found_missing) {
                        return false;
                    }
                    missing = el;
                    found_missing = true;
                } else {
                    sum = sum + *cell_set.begin();
                }
            }
            if (found_missing) {
                idea.cells[missing] = ValueSet{con->desired_sum - sum};
                return true;
            }
            return false;
        }
    };

    template <class CoordSet, class ValueSet>
    class SimpleUniqueStrategy : public Strategy<CoordSet, ValueSet> {
    public:
        bool advance(SolutionIdea<CoordSet, ValueSet> &idea) const override {
            auto con = static_cast<UniqueConstraint<CoordSet, ValueSet>*>(this->constraint);
            bool result = false;
            ValueSet found_values;
            for (auto el : con->area) {
                auto &cell_set = idea.cells[el];
                if (cell_set.size() == 1) {
                    found_values.insert(*cell_set.begin());
                }
            }
            for (auto el : con->area) {
                auto &cell_set = idea.cells[el];
                if (cell_set.size() != 1) {
                    for (auto val : found_values) {
                        cell_set.erase(val);
                        result = true;
                    }
                }
            }
            return result;
        }
    };
}
