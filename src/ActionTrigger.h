#ifndef ACTION_TRIGGER_H
#define ACTION_TRIGGER_H

class Component;

class ActionTrigger {
public:
    ActionTrigger(int t, Component *c, void(Component::*a)()) : trigger(t), pComponent(c), action(a) {}
    ~ActionTrigger() {}

    Component *pComponent;
    void(Component::*action)();
    int trigger;
};

#endif //ACTION_TRIGGER_H