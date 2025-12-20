#ifndef SKIP_ACTION_HPP
#define SKIP_ACTION_HPP

enum SkipAction{
    Skip = 0,
    Move,
    Copy,
    Test,
    Force,
    None // not selected, 
};

static const char *skipActionString[] = {
    "skip",
    "move",
    "copy",
    "test",
    "force",
    "none"
};

#endif // SKIP_ACTION_HPP