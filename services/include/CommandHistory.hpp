#pragma once
#include "../../models/include/Command.hpp"
#include <vector>
#include <functional>
#include <memory>

class CommandHistory {
private:
    std::vector<std::unique_ptr<Command>> history;
    size_t currentIndex;
    static const size_t MAX_HISTORY_SIZE = 50;

public:
    CommandHistory();
    
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    void clear();
};

