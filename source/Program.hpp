#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#define IN_DEBUG false

#define VERSION "1.5.7"
static const char *versionLine = "FFmpegRec version " VERSION "\n";

class Program{
public:
    static int run(int argc, const char **argv);
};


#endif // PROGRAM_HPP