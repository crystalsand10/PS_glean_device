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



class trial: public Device_base{
public:
    // constructor requires processor name and output destination
    trial(const std:: string& id, const Output_tee& ot);
    
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
    Symbol current_pointed_to_object;
    
};

#endif /* defined(__TLD_device__trial__) */
