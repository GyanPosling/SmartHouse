#include "../include/CommandHistory.hpp"

CommandHistory::CommandHistory() = default;

CommandHistory::~CommandHistory() {
    clear();
}

void CommandHistory::clearStack(stack<Command*>& s) {
    while (!s.empty()) {
        delete s.top();
        s.pop();
    }
}

void CommandHistory::executeCommand(Command* command) {
    if (!command) {
        return;
    }

    clearStack(redoStack);

    command->execute();
    undoStack.push(command);
}

void CommandHistory::undo() {
    if (canUndo()) {
        Command* cmd = undoStack.top();
        undoStack.pop();
        cmd->undo();
        redoStack.push(cmd);
    }
}

void CommandHistory::redo() {
    if (canRedo()) {
        Command* cmd = redoStack.top();
        redoStack.pop();
        cmd->execute();
        undoStack.push(cmd);
    }
}

bool CommandHistory::canUndo() const {
    return !undoStack.empty();
}

bool CommandHistory::canRedo() const {
    return !redoStack.empty();
}

void CommandHistory::clear() {
    clearStack(undoStack);
    clearStack(redoStack);
}
