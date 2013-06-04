//
//  trial.cpp
//  TLD_device
//
//  Created by Sandeep Kaur on 8/05/13.
//  Last modified: 2013, May 22
//


#include "TLD_device.h"
#include "GLEANKernel/Device_exception.h"
#include "GLEANKernel/Glean_standard_symbols.h"
#include "GLEANKernel/Output_tee_globals.h"
#include "GLEANKernel/Utility_templates.h"	// for Delete_second
#include "GLEANKernel/Numeric_utilities.h"	// for random_int
#include "GLEANKernel/Symbol_utilities.h"	// for concatenate_to_Symbol
#include <iostream>
#include "GLEANKernel/Widgets.h"

using namespace std;

const Symbol Button_c("SubmitButton");
const Symbol Field_c("EnterAllergy");


const GU::Size default_button_size(50, 20);
const GU::Size default_textField_size(100, 20);

// constructor
TLD_device::TLD_device(const std::string& id, Output_tee& ot) : Device_base(id, ot), submit_button_clicked(false) {
    Widget::set_add_widget_type_property(true); // Now "Type Button" and "Type Field" are properties of objects.
}

void TLD_device::initiliaze(){
    Device_base::initialize();
    clear_display();
    
    current_pointed_to_object_name = Nil_c;
    keyboard_focus_field = Nil_c;
    create_initial_display();
    
}



void TLD_device::clear_display(){
    formElements.clear();
    screen_ptr = 0;
    submit_ptr = 0;
}



void TLD_device::create_initial_display(){
    screen_ptr = new Screen_widget(this, Symbol("Screen"), GU::Point(0, 0), Widget::get_screen_pixel_size());
    screen_ptr->add_widget(cursor_ptr = new Cursor_widget(this, GU::Point(250, 250), GU::Size(20, 20)));
    
    create_field(this, Field_c, GU::Point(500, 200), "Enter_allergy"); // Labelled as Enter_allergy
    create_button(this, Button_c, GU::Point(500, 300), "Submit_allergy", false); // Labelled as Submit_allergy
    
    
    // formElements[Field_c]->set_property(Position_c, Top_c);
    
    
}


void TLD_device::create_field(TLD_device * device_ptr, const Symbol& name, GU::Point location, const Symbol& label){
    
    Smart_Pointer<Field_widget> ptr = new Field_widget(device_ptr, name, location, default_textField_size);
    formElements[name] = ptr;
    screen_ptr->add_widget(ptr);
    
}



void TLD_device::create_button(TLD_device * device_ptr, const Symbol& name, GU::Point location, const Symbol& label, bool state){
    submit_ptr = new Button_widget(device_ptr, name, location, default_button_size, label, Red_c, Green_c, state);
     // formElements[name] = ptr;
    screen_ptr->add_widget(submit_ptr);
    submit_ptr->present(); 
}



void TLD_device::display() const{
    screen_ptr->present();
    output_display();
}

void TLD_device::output_display() const{
    device_out << "--------------\n" <<processor_info() << " Display: " << endl;
    
    for(formElements_t::const_iterator it=formElements.begin(); it!=formElements.end(); ++it){
        device_out << (it->second)->get_name()
        << ' ' << (it->second)->get_location() << ' ';
        
    }
    // make sure submit ptr gets initlialised somewhere.
    device_out << submit_ptr->get_name() << ' ' << submit_ptr->get_location() << ' ' << submit_ptr->get_state() << endl;
    
    
    device_out << "Cursor pointing at " << current_pointed_to_object_name << endl;
    device_out << "-------------------" << endl;
    
}


// If user points to an object, remember waht was pointed-to and modify the display accordingly.
void TLD_device::handle_Point_event(const Symbol& target_name){
    if(Trace_out && get_trace())
        Trace_out << processor_info() << " Point_to: " << target_name << endl;
    if(target_name == "Absent" || target_name == Nil_c)
        Normal_out << processor_info() << " Point_to non-existant object: " << target_name <<endl;
    current_pointed_to_object_name = target_name;
    Smart_Pointer<Widget> ptr = screen_ptr->get_widget_ptr(current_pointed_to_object_name);
    if(ptr) {
        GU::Point new_loc = ptr->get_location();
        cursor_ptr->set_location(new_loc);
    }
    
    set_visual_object_property(Cursor_name_c, Pointing_to_c, target_name);
    output_display();
}

// handle click event separately for button and text fields.

void TLD_device::handle_Click_event(const Symbol& clicked_name){
    if(Trace_out && get_trace())
        Trace_out << processor_info() << " Click: " << clicked_name << " on " << current_pointed_to_object_name << endl;
    
    
    if(current_pointed_to_object_name == submit_ptr->get_name() && submit_ptr->get_state() == false){
        // End simulation
        stop_simulation();
        return; 
    }
    else{
        formElements_t::iterator it = formElements.find(current_pointed_to_object_name);
        if(it == formElements.end())
            throw Device_exception(this, "Click-on unrecognised object");
        
        Smart_Pointer<Field_widget> current_field_pointer = it->second;
        
        keyboard_focus_field = current_pointed_to_object_name;
        
        
    }
    
    
    
    output_display();
    
}
void TLD_device::handle_Type_In_event(const Symbol& type_in_string){
    // get the keyboard field and set its property that we have just typed in.
    if(keyboard_focus_field == Nil_c){
         throw Device_exception(this, "keyboard_focus_field not set");
    }
    
    formElements_t::iterator it = formElements.find(keyboard_focus_field);
    (it->second)->set_string(type_in_string);
    
    
}



void TLD_device::handle_Delay_event(const Symbol& type, const Symbol& datum, const Symbol&object_name, const Symbol& property_name, const Symbol& property_value){
    
}




