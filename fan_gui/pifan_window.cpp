#include "pifan_window.hpp"
#include <iostream>
#include <pifanconfig.h>

PiFanWindow::PiFanWindow() : m_button1("Button 1"), m_button2("Button 2") {
   
   // Set the title of the new window
   set_title("Pi Fan Control");
   
   // Set border width of the window
   set_border_width(10);
   
   // Add the box to the main window
   add(m_box1);
   
   // Call the "on_button_clicked" function when the button is clicked
   m_button1.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &PiFanWindow::on_button_clicked), "button 1"));
   
   // Pack the button into the box and show it
   m_box1.pack_start(m_button1);
   m_button1.show();
   
   // Set up signal handler for button2
   m_button2.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &PiFanWindow::on_button_clicked), "button 2"));
   
   m_box1.pack_start(m_button2);
   m_button2.show();
   m_box1.show();
}

PiFanWindow::~PiFanWindow() {

}

/*
    The button signal handler
*/
void PiFanWindow::on_button_clicked(Glib::ustring data) {
    std::cout << "PiFanWindow - " << data << " was pressed." << std::endl;
}
