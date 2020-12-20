#include <future>
#include <vector>
#include <iostream>
#include "gui_mode.hpp"
#include "server_mode.hpp"

enum run_mode {
    gui,
    server,
    help
};

struct commandline_options {
    run_mode mode;
};

commandline_options parse_commandline_args(std::vector<std::string>&& args) {
    commandline_options ret;
    if(args.empty()) {
        ret.mode = run_mode::gui;
    }
    for(const auto& x: args) {
        if(x=="-h"||x=="--help") {
            ret.mode = run_mode::help;
            return ret; //ignore everything else
        } else if(x=="-g"||x=="--gui") {
            ret.mode = run_mode::gui;
            return ret; //ignore everything else
        } else if(x=="-s"||x=="--server") {
            ret.mode = run_mode::server;
        } else {
            ret.mode = run_mode::help;
            std::cerr<<"Unrecognized commandline option: "<<x<<std::endl;
            return ret; //ignore everything else due to the parsing error
        }
    }
    return ret;
}

int main(int argc, char** argv) {
    std::vector<std::string> args;
    for(int i=1;i<argc;i++) {
        args.push_back(std::string(argv[i]));
    }

    auto cmdln_options = parse_commandline_args(std::move(args));

    switch(cmdln_options.mode) {
        case run_mode::gui:{
            gui_mode();
            break;
        }
        case run_mode::server:{
            std::cerr<<"Server mode not implemented."<<std::endl;
            server_mode();
            //return 1;
            break;
        }
        case run_mode::help:{
            std::cout<<"Gomoku"<<std::endl<<std::endl;
            std::cout<<"\t--help   (-h)   Show this help"<<std::endl;
            std::cout<<"\t--server (-s)   Run in server mode"<<std::endl;
            std::cout<<"\t--gui    (-g)   Run in gui mode (default)"<<std::endl;
            break;
        }
        default:{
            std::cerr<<"Internal logic error"<<std::endl;
            return 3;
        }
    }
    return 0;
}