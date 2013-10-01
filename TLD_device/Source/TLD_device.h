#ifndef TLD_DEVICE_H
#define TLD_DEVICE_H

#include "GLEANKernel/Device_base.h"
#include "GLEANKernel/Smart_Pointer.h"
#include "GLEANKernel/Widgets.h"
#include <map>

class TLD_device;
// a simple device that puts Button widgets on the simulated display
// the color can be read and changed, whereupon the display changes.


class TLD_device : public Device_base {
public:
	TLD_device(const std::string& id, Output_tee& ot);
    
	virtual void initialize();
	virtual void handle_Start_event();
	virtual void handle_Point_event(const Symbol& target_name);
	virtual void handle_Click_event(const Symbol& button_name);
    
	virtual void handle_Type_In_event(const Symbol& type_in_string);
    
    virtual void handle_Keystroke_event(const Symbol& key_name);
    
/*	virtual void handle_Delay_event(const Symbol& type, const Symbol& datum,
                                    const Symbol& object_name, const Symbol& property_name, const Symbol& property_value);*/ 
private:
	Smart_Pointer<Screen_widget> screen_ptr;
	// a container of pointers to buttons
	typedef std::map<Symbol, Smart_Pointer<Button_widget> > buttons_t;
	buttons_t buttons;
    buttons_t menuItems;
    
    typedef std::map<Symbol, Smart_Pointer<Labeled_field_widget> >labeledFields_t;
    labeledFields_t labeledFields; 
    
    typedef std::map<Symbol, Smart_Pointer<Field_widget> > fields_t;
	fields_t fields;
	
    typedef std::map<Symbol, Smart_Pointer<Polygon_widget> > polygons_t;
    polygons_t polygons;
    
    typedef std::map<Symbol, Smart_Pointer<Object_widget> > objects_t;
    objects_t objects;
 
    typedef std::map<Symbol, Smart_Pointer<Label_widget> > labels_t;
    labels_t labels;
  
    typedef std::map<Symbol, Smart_Pointer<Window_widget> > windows_t;
    windows_t windows;
  
    
    typedef std::map<Symbol, Symbol> scheduledMedications_t;
    scheduledMedications_t scheduledMedications;
    
    typedef std::map<Symbol, Symbol> prnMedications_t;
    prnMedications_t prnMedications;
    
    
    Smart_Pointer<Cursor_widget> cursor_ptr;
	
	
	// Smart_Pointer<Object_widget> blip_ptr;	// at most one Blip can exist at a time
	// int blip_counter;	// counts the number of time a blip was clicked on
    
	Symbol current_pointed_to_object_name;	// holds name of current button pointed to
    Symbol current_searched_medication; // holds name of current medication searched for, will be used for creating button later;
    
    
    
    
	void create_initial_display();
	
    void create_button(TLD_device * dev_ptr, const Symbol& name, GU::Point location, GU::Size size, const Symbol& label, bool state, Smart_Pointer<Screen_widget> screenName , bool should_present, const Symbol& color_on, const Symbol& color_off);
	
    void create_Field(Device_base * dev_ptr, const Symbol& widget_name, GU::Point location, GU::Size size, Symbol new_string, Symbol color, bool should_present);
    
    
    void create_labeledField(Device_base * dev_ptr, const Symbol& widget_name,GU::Point location, GU::Size label_size, GU::Size field_size, const Symbol& label, bool should_present, bool is_dosageField);
    
    void create_polygon(Device_base * dev_ptr, const Symbol& widget_name, const GU::Polygon& polygon, const Symbol& color, bool should_present);
    
    void create_Object(Device_base * dev_ptr, const Symbol& widget_name, GU::Point location, GU::Size size,
                       const Symbol& color, const Symbol& shape, const Symbol& label, bool should_present);
    
    void create_label(Device_base * dev_ptr, const Symbol& widget_name, GU::Point location, GU::Size size, const Symbol& label, const Symbol& color, bool should_present);
    
	void clear_display();
	void output_display() const;
    
    
    
    void createMenuItems();
    void deleteMenuItems(); 
    void create_homeScreen_display(bool first_display);
    void clear_objects_on_screen();


    bool iequals(const std::string& a, const std::string& b);
    void display_prescribedMedications();
    void assign_initial_dosage(Smart_Pointer<Labeled_field_widget>);
    void createMenuItems2(); // for maximum of doses/Tablet
    void createMenuItems3(); // for scheduled medication route options;
    void createMenuItems4(); // for "For" options in the scheduled medications screen.
    
    
    void create_warning_display();
    
    
    
    
    void create_medications_display(bool first_screen, bool second_screen, bool second_screen_searchResults, bool second_screen_formSelection, bool third_screen, bool third_screen_PRN, bool fourth_screen);
    
    
    void create_allergies_display(bool second_screen,  bool isOption6, bool isOption3);
    
    
    void display_medications_FormStrength(std::string medication);
};

#endif
