#include "../include/CommandHistory.hpp"

CommandHistory::CommandHistory() : currentIndex(0) {}

void CommandHistory::executeCommand(std::unique_ptr<Command> command) {
    // Удаляем все команды после текущего индекса (если были отмены)
    if (currentIndex < history.size()) {
        history.erase(history.begin() + currentIndex, history.end());
    }
    
    // Выполняем команду
    command->execute();
    
    // Добавляем в историю
    history.push_back(std::move(command));
    currentIndex = history.size();
    
    // Ограничиваем размер истории
    if (history.size() > MAX_HISTORY_SIZE) {
        history.erase(history.begin());
        currentIndex--;
    }
}

void CommandHistory::undo() {
    if (canUndo()) {
        currentIndex--;
        history[currentIndex]->undo();
    }
}

void CommandHistory::redo() {
    if (canRedo()) {
        history[currentIndex]->execute();
        currentIndex++;
    }
}

bool CommandHistory::canUndo() const {
    return currentIndex > 0;
}

bool CommandHistory::canRedo() const {
    return currentIndex < history.size();
}

void CommandHistory::clear() {
    history.clear();
    currentIndex = 0;
}

