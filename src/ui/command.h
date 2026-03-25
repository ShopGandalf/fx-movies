#pragma once

#include <functional>
#include <memory>

namespace fx {

// WPF-style ICommand interface.
// Implement execute() to define what happens; override can_execute() to gate execution.
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual bool can_execute() const { return true; }
};

// Lightweight command backed by lambdas — no subclassing required for simple cases.
//
// Usage:
//   auto cmd = std::make_shared<RelayCommand>(
//       [&]() { open_help(); },
//       [&]() { return help_available; }
//   );
class RelayCommand : public ICommand {
public:
    using ExecuteFn    = std::function<void()>;
    using CanExecuteFn = std::function<bool()>;

    explicit RelayCommand(ExecuteFn execute_fn, CanExecuteFn can_execute_fn = nullptr)
        : execute_fn_(std::move(execute_fn))
        , can_execute_fn_(std::move(can_execute_fn))
    {}

    void execute() override {
        if (execute_fn_) execute_fn_();
    }

    bool can_execute() const override {
        if (can_execute_fn_) return can_execute_fn_();
        return true;
    }

private:
    ExecuteFn    execute_fn_;
    CanExecuteFn can_execute_fn_;
};

} // namespace fx
