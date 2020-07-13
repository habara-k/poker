#include "record.h"

#include <cassert>

namespace poker {
    Record::Record(int player_id, const Action& action)
            : type_(RecordType::kAction), player_id_(player_id), action_(action) {}

    Record::Record(Stage stage)
            : type_(RecordType::kStage), stage_(stage) {}

    RecordType Record::type() const {
        return type_;
    }

    int Record::player_id() const {
        assert(type_ == RecordType::kAction);
        return player_id_.value();
    }

    Action Record::action() const {
        assert(type_ == RecordType::kAction);
        return action_.value();
    }

    Stage Record::stage() const {
        assert(type_ == RecordType::kStage);
        return stage_.value();
    }
}