#include <dandy/shell/DShell.hpp>
#include <optparse/optparse.h>


int d_shellcommand_example(const DShellCommandContext* ctx)
{
    // see https://github.com/skeeto/optparse
    struct optparse_long longopts[] = {
        {"help", 'h', OPTPARSE_NONE},
        {"color", 'c', OPTPARSE_OPTIONAL},
        {"address", 'a', OPTPARSE_REQUIRED},
        {0}
    };

    struct optparse options;
    const char *color = nullptr;
    char *arg;
    int option;
    unsigned int address = 0;
    const char* name = ctx->argv[0];

    ctx->stdOut->printf("print argv\n");
    for (int i = 0; i < ctx->argc; i++)
    {
        ctx->stdOut->printf("arg[%d]: %s\n", i, ctx->argv[i]);
    }

    optparse_init(&options, ctx->argv);
    while ((option = optparse_long(&options, longopts, NULL)) != -1) {
        switch (option) {
        case 'h':
            ctx->stdErr->printf(
                "usage: %s [-h] [-c [COLOR]] [-a ADDRESS] MSG...\n"
                " -h --help\t\tshow this help message and exit\n"
                " -c --color\t\tchoice a color the text [red|green|blue]\n"
                " -a --address\t\taddress value\n", name);
            return EXIT_SUCCESS;
        case 'c':
            color = options.optarg ? options.optarg : "red";
            break;
        case 'a':
            address = x_strtouint32(options.optarg, 0, NULL);
            break;
        case '?':
            ctx->stdErr->printf("%s: %s\n", ctx->argv[0], options.errmsg);
            return EXIT_FAILURE;
        default:
            break;
        }
    }

    if (!color)
        color = X_ANSI_COLOR_RESET;
    else if (::strcmp(color, "red") == 0)
        color = X_ANSI_COLOR_RED;
    else if (::strcmp(color, "green") == 0)
        color = X_ANSI_COLOR_GREEN;
    else if (::strcmp(color, "blue") == 0)
        color = X_ANSI_COLOR_BLUE;
    else
        color = X_ANSI_COLOR_RESET;

    /* Print remaining arguments. */
    while ((arg = optparse_arg(&options)))
    {
        ctx->stdOut->printf(
            "%saddress:0x%08X msg:%s%s\n",
            color, address, arg, X_ANSI_COLOR_RESET);
    }

    return EXIT_SUCCESS;
}
