#include "pifan_window.hpp"
#include <gtkmm/application.h>

int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv, "dev.bdavidson.pifangtk");
    
    PiFanWindow piFanWindow;
    
    // Show window and returns when it's closed
    return app -> run(piFanWindow);
}
