#ifndef POKER_RECORD_H
#define POKER_RECORD_H

#include <vector>

#include "action.h"
#include "stage.h"

namespace poker {

    enum class RecordType {
        kStage, kAction
    };

    class Record {
        const RecordType type_;
        const std::optional<int> player_id_;
        const std::optional<Action> action_;
        const std::optional<Stage> stage_;
    public:
        Record(int player_id, const Action& action);
        explicit Record(Stage stage);

        [[nodiscard]] RecordType type() const;
        [[nodiscard]] int player_id() const;
        [[nodiscard]] Action action() const;
        [[nodiscard]] Stage stage() const;
    };
}

#endif //POKER_RECORD_H
