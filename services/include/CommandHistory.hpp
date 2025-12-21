#pragma once
#include "../../models/include/Command.hpp"
#include <stack>

using namespace std;

class CommandHistory {
private:
    stack<Command*> undoStack;
    stack<Command*> redoStack;
    
    void clearStack(stack<Command*>& s);

public:
    CommandHistory();
    ~CommandHistory();
    
    void executeCommand(Command* command);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    void clear();
};
