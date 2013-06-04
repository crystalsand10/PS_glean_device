//
//  trial.h
//  TLD_device
//
//  Created by Sandeep Kaur on 2013, May 8.
//
//  Last revised: 2013, May 14
//

#ifndef __TLD_device__trial__
#define __TLD_device__trial__

#include <iostream>
#include <string>
#include "GLEANKernel/Device_base.h"
#include "GLEANKernel/Symbol.h"
#include "GLEANKernel/Output_tee.h"
#include "GLEANKernel/Smart_Pointer.h"
#include "GLEANKernel/Widgets.h"
#include <map>


class TLD_device: public Device_base{
public:
    TLD_device(const std::string& id, Output_tee& ot);     // constructor requires processor name and output destination
    
    virtual void initiliaze();
    virtual void display() const;
    
    // over-riding input handlers
    
    virtual void handle_Delay_event(const Symbol& type, const Symbol& datum, const Symbol& object_name, const Symbol& property_name, const Symbol& property_value);
    
    
    virtual void handle_Type_In_event(const Symbol& type_in_string);
    
    virtual void handle_Point_event(const Symbol& target_name);
    
    virtual void handle_Click_event(const Symbol& button_name);
    
    
    
private:
    bool submit_button_clicked;
    Symbol keyboard_focus_field; // target for keyboard input; multiple targets might be required for a form which has lots of things.
    
    Symbol current_pointed_to_object_name;
    
    Smart_Pointer<Screen_widget> screen_ptr;
    Smart_Pointer<Cursor_widget> cursor_ptr;

    
    
    Smart_Pointer<Button_widget> submit_ptr; // Is this required? (aims to be a substitute for blip pointer).
    
    
    
    // Defining a location to store the form elements (all fields and buttons)
    typedef std::map<Symbol, Smart_Pointer<Field_widget> > formElements_t;
	formElements_t formElements;
    
    
    
    
    // Added things not in Guide's .h file.
    
    void clear_display();
    void create_initial_display();
    void create_button(TLD_device * device_ptr, const Symbol& name, GU::Point location, const Symbol& label, bool state);
    void create_field(TLD_device * device_ptr, const Symbol& name, GU::Point location, const Symbol& label);
    void output_display() const;
};

#endif /* defined(__TLD_device__trial__) */
