#include "TLD_device.h"
#include "GLEANKernel/Device_exception.h"
#include "GLEANKernel/Glean_standard_symbols.h"
#include "GLEANKernel/Output_tee_globals.h"
#include "GLEANKernel/Utility_templates.h"	// for Delete_second
#include "GLEANKernel/Numeric_utilities.h"	// for random_int
#include "GLEANKernel/Symbol_utilities.h"	// for concatenate_to_Symbol
#include <iostream>
#include <iomanip>

using namespace std;


/* All the buttons in the display */
const Symbol ButtonAllergy_c("Button_allergy");
const Symbol ButtonMedications_c("Button_medications");


/* All the Fields in the display */
const Symbol Field_AllergySubstance_c("AllergySubstance");
const Symbol Field_AllergyComments_c("AllergyComments");

/* Strings to print out, this one is for allergy // change name */ 
std::stringstream Main_screen_string1;



TLD_device::TLD_device(const std::string& id, Output_tee& ot) :
Device_base(id, ot)
{
	// If one object property refers to another object, GLEAN currently requires that
	// the physical name be same as local names ...
	Widget::set_add_widget_type_property(true);  // e.g. so "Type Button" will be a property of an object
}



void TLD_device::initialize()
{
	Device_base::initialize();
	clear_display();
	
	current_pointed_to_object_name = Nil_c;
	// if  a blip was left up, get rid of it
//	blip_ptr = 0;
//	blip_counter = 0;
	// set up the initial display
	create_initial_display();
}

/* Creates the initial display when the simulation starts */
void TLD_device::create_initial_display()
{
	screen_ptr = new Screen_widget(this, Symbol("Screen"), GU::Point(0, 0), Widget::get_screen_pixel_size());
	screen_ptr->add_widget(cursor_ptr = new Cursor_widget(this, GU::Point(250, 250), GU::Size(20, 20)));
    create_homeScreen_display(false, false);
}



/* Creates the initial display when the simulation starts */ 
void TLD_device::handle_Start_event()
{
	screen_ptr->present();
	
	// schedule the first blip appearance sometime between 2 sec and 3 secs from now
    //	int delay = 2000 + random_int(3000);
	// schedule the first blip appearance sometime between 6 sec and 9 secs from now
	// give the button pressing time to get done
	// int delay = 6000 + random_int(3000);
	// schedule_delay_event(delay, "Blip_appear", "");
    
	output_display();
    
    
}






void TLD_device::create_button(TLD_device * dev_ptr, const Symbol& name, GU::Point location, GU::Size size, const Symbol& label, bool state,  Smart_Pointer<Screen_widget> screenName, bool should_present,const Symbol& color_on, const Symbol& color_off)
{
	Smart_Pointer<Button_widget> ptr = new Button_widget(dev_ptr, name, location, size, label, color_on, color_off, state);
	buttons[name] = ptr;
	screenName->add_widget(ptr);
    
    if(should_present == true){
        ptr->present();
    }
}

void TLD_device::create_polygon(Device_base * dev_ptr, const Symbol& widget_name, const GU::Polygon& polygon, const Symbol& color, bool should_present) {
    Smart_Pointer<Polygon_widget> ptr = new Polygon_widget(dev_ptr, widget_name, polygon, color);
    ptr->set_add_widget_type_property(true);
    polygons[widget_name] = ptr; 
    screen_ptr->add_widget(ptr);
    
    if(should_present == true){
        ptr->present();
    }
}

void TLD_device::create_Field(Device_base * dev_ptr, const Symbol& widget_name, GU::Point location, GU::Size size , Symbol new_string, Symbol color, bool should_present){
    Smart_Pointer<Field_widget> ptr = new Field_widget(dev_ptr, widget_name, location, size, new_string, color);
   // ptr->set_add_widget_type_property(true);
   // ptr->set_string("blah");
    fields[widget_name] = ptr;
    screen_ptr->add_widget(ptr);
  //  screen_ptr->present_property("Name", "Substances");
 //   Trace_out << " name is .......... " << ptr->get_name() << " .......... " << endl;
    if(should_present == true){
        ptr->present();
    }
    
}

void TLD_device::create_label(Device_base * dev_ptr, const Symbol& widget_name, GU::Point location, GU::Size size, const Symbol& label, const Symbol& color, bool should_present){
    Smart_Pointer<Label_widget> ptr = new Label_widget(dev_ptr, widget_name, location, label, Red_c);
  //  ptr->set_add_widget_type_property(true);
    labels[widget_name] = ptr;
    screen_ptr->add_widget(ptr);
    
    if(should_present == true){
        ptr->present();
    }
}

void TLD_device::create_Object(Device_base * dev_ptr, const Symbol& widget_name, GU::Point location, GU::Size size,
                               const Symbol& color, const Symbol& shape, const Symbol& label , bool should_present){
    Smart_Pointer<Object_widget> ptr = new Object_widget(dev_ptr, widget_name, location, size);
    ptr->set_property(Color_c, color);
    ptr->set_property(Shape_c, shape);
    ptr->set_property(Text_c, label);
    objects[widget_name] = ptr;
    screen_ptr->add_widget(ptr);
    
    if(should_present == true){
        ptr->present();
    }
    
}





// Output the state of the buttons and the cursor as a text display
void TLD_device::output_display() const
{
	device_out << "--------------------\n" << processor_info() << " Display: " << endl;
	for(buttons_t::const_iterator it = buttons.begin(); it != buttons.end(); ++it) {
		device_out << (it->second)->get_name()
		<< ' ' << (it->second)->get_location() << ' ' << (it->second)->get_state() << endl;
    }
    
	device_out << "Cursor pointing at " << current_pointed_to_object_name << endl;
	device_out << "--------------------" << endl;;	
}




/* This function clears everything including the screen and the cursor */
void TLD_device::clear_display()
{
	buttons.clear();
    fields.clear();
	screen_ptr = 0;
    cursor_ptr = 0;
    labels.clear();
    objects.clear();
    polygons.clear();
    
}




/* For each of the various objects on the screen, 
 * depresents them on the screen; 
 * removes them from the screen_ptr; 
 * finally clears them from its respective type array
 * This is better for clearing objects without also clearing the screen and the cursor. 
 */
void TLD_device::clear_objects_on_screen(){
    
    for(buttons_t::const_iterator it = buttons.begin(); it != buttons.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second)); 
    }
    
    
    for(fields_t::const_iterator it = fields.begin(); it != fields.end(); ++it) {
        (it->second)->depresent();
         screen_ptr->remove_widget((it->second));
    }
    
    
    for(polygons_t::const_iterator it = polygons.begin(); it != polygons.end(); ++it) {
        (it->second)->depresent();
         screen_ptr->remove_widget((it->second));
    }
    
    
    for(objects_t::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        (it->second)->depresent();
         screen_ptr->remove_widget((it->second));
    }
    
    
    for(labels_t::const_iterator it = labels.begin(); it != labels.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second));
        
    }
    
    buttons.clear();
    fields.clear();
    polygons.clear();
    objects.clear();
    labels.clear(); 
}




/* Creates the home screen from which the user can click on various options to add allergies, medication etc. */
void TLD_device::create_homeScreen_display(bool first_display, bool allergy_string_display){
    
    
    create_button(this, ButtonAllergy_c, GU::Point(500, 200), GU::Size(35, 20),  "Add", true, screen_ptr, first_display, Gray_c, LightGray_c);
    buttons[ButtonAllergy_c]->set_property("Name", "Add");
    
    
    create_button(this, Symbol("button2"),  GU::Point(10, 230), GU::Size(200, 20), "Medications on admission", false, screen_ptr , first_display, Gray_c, LightGray_c);
    
    create_button(this, Symbol("button3"),  GU::Point(10, 270), GU::Size(120, 20), "Chart history", false, screen_ptr , first_display, Gray_c, LightGray_c);
    
    create_button(this, Symbol("button4"),  GU::Point(10, 300), GU::Size(120, 20), "Script history", false, screen_ptr , first_display, Gray_c, LightGray_c);
    
    create_button(this, Symbol("button5"),  GU::Point(10, 330), GU::Size(140, 20), "Patient history", false, screen_ptr , first_display, Gray_c, LightGray_c);
    
    
    create_button(this, ButtonMedications_c, GU::Point(10, 200), GU::Size(100, 20),  "Medications", true, screen_ptr, first_display, Gray_c, LightGray_c);
    buttons[ButtonMedications_c]->set_property("Name", "Medications");
    
    create_Object(this, "Object1", GU::Point(255, 200), GU::Size(240, 20), RoyalBlue_c, Filled_Rectangle_c, "Allergies and Intolerances", first_display);
    // screen_ptr->get_size().v ;
    
    std::vector<GU::Point> in_vertices1 {GU::Point(1, 190), GU::Point(650 , 190)}; // horizontal line
    create_polygon(this, Symbol("Polygon1"), in_vertices1, DarkGray_c, first_display);
    
    
    std::vector<GU::Point> in_vertices2 {GU::Point(250, 190), GU::Point( 250, 550)}; // vertical line
    create_polygon(this, Symbol("Polygon2"), in_vertices2, DarkGray_c, first_display);
    
    std::vector<GU::Point> in_vertices3 {GU::Point(1, 260), GU::Point( 250, 260)};
    create_polygon(this, Symbol("Polygon3"), in_vertices3, DarkGray_c, first_display);
    
    std::vector<GU::Point> in_vertices4 {GU::Point(250, 300), GU::Point(  650, 300)};
    create_polygon(this, Symbol("Polygon4"), in_vertices4, DarkGray_c, first_display);
    
    std::vector<GU::Point> in_vertices5 {GU::Point(542, 190), GU::Point(  542, 300)};
    create_polygon(this, Symbol("Polygon5"), in_vertices5, DarkGray_c, first_display);
    
    
    create_Object(this, "Object2", GU::Point(255, 310), GU::Size(300, 20), RoyalBlue_c, Filled_Rectangle_c, "Medications on Admission Status", first_display);
    
    create_Object(this, "Object3", GU::Point(255, 360), GU::Size(300, 20), RoyalBlue_c, Filled_Rectangle_c, "Inpatient Medications", first_display);
    
    
    if(allergy_string_display == true) {
        // create a label perhaps and display Main_screen_string1
        
        create_label(this, "Allergy_information string", GU::Point(258, 230), GU::Size(100, 20), Main_screen_string1.str(), Red_c, true);
    }
    
    
}



/* Creates the display when user clicks on Add. Second screen appears when the user clicks on a selection, in this case - non-drug allergy */ 
void TLD_device::create_allergies_display(bool isFirstScreen, bool isOption3, bool isOption6){

    
    Trace_out << processor_info() << " Now new screen for entering allergy information 2.2 " << endl;
    
    if( isFirstScreen == true) {

        std::vector<GU::Point> in_vertices1 {GU::Point(1, 190), GU::Point(650 , 190)}; // horizontal line
        create_polygon(this, Symbol("Allergy_Polygon1"), in_vertices1, DarkGray_c, true);
        
        std::vector<GU::Point> in_vertices2 {GU::Point(318, 190), GU::Point( 318, 550)}; // vertical line
        create_polygon(this, Symbol("Allergy_polygon2"), in_vertices2, DarkGray_c, true);
    
        create_button(this, "Allergy_continue", GU::Point(20, 130), GU::Size(50, 15), "Continue", true, screen_ptr, true, Gray_c, LightGray_c);
        buttons[Symbol("Allergy_continue")]->set_property("Name", "Continue");
        
        create_button(this, "Allergy_cancel", GU::Point(100, 130), GU::Size(50, 15), "Cancel", false, screen_ptr, true, Gray_c, LightGray_c);
        
        std::vector<GU::Point> in_vertices3 {GU::Point(1, 150), GU::Point(650 , 150)}; // horizontal line
        create_polygon(this, Symbol("Allergy_Polygon3"), in_vertices3, DarkGray_c, true);
        
        
        create_button(this, "Allergy_option1", GU::Point(10, 200), GU::Size(10, 10), "  Patients Allergy Status", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, "Allergy_option2", GU::Point(10, 220), GU::Size(10, 10), "  No known allergies or intolerances", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, "Allergy_option3", GU::Point(10, 240), GU::Size(10, 10), "  Class allergy", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, "Allergy_option4", GU::Point(10, 260), GU::Size(10, 10), "  Drug allergy", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, "Allergy_option5", GU::Point(10, 280), GU::Size(10, 10), "  Drug intolerances", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, "Allergy_option6", GU::Point(10, 300), GU::Size(10, 10), "  Non drug allergy", true, screen_ptr, true, LightGray_c, Black_c);
        
        buttons[Symbol("Allergy_option3")]->set_property("Name", "ClassAllergy");
        buttons[Symbol("Allergy_option4")]->set_property("Name", "DrugAllergy");
        buttons[Symbol("Allergy_option6")]->set_property("Name", "NonDrug");
    }

    if( isOption6 == true && isOption3 == true) {
    
        create_label(this, "Allergy_label_Comments", GU::Point(350, 400), GU::Size(100, 20), "Comments", Green_c, true);
        
        create_Field(this, Field_AllergyComments_c, GU::Point(420, 400), GU::Size(100, 50), " ", Black_c, true);
        
        
        return; 
    }
    
    
    if( isOption6 == true){
        
        Trace_out << processor_info() << " Now new screen for entering allergy information 2.1 " << endl;
        
        create_label(this, "Allergy_label_Substance", GU::Point(340, 230), GU::Size(100, 20), "Substance", Red_c, true);
        create_label(this, "Allergy_label_Comments", GU::Point(350, 400), GU::Size(100, 20), "Comments", Green_c, true);
        create_Field(this, Field_AllergySubstance_c, GU::Point(420, 230),  GU::Size(100, 20)," ", Black_c, true);
        create_Field(this, Field_AllergyComments_c, GU::Point(420, 400), GU::Size(100, 50), "", Black_c, true);
        
    }
    
    if( isOption3 == true){
        // label, field, button
        // clicks on search, choses, enters comments rash.
        
        Trace_out << processor_info() << " Now new screen for entering allergy information 2.2 " << endl;
        
        create_label(this, "Allergy_class_name", GU::Point(10, 320), GU::Size(30, 30), "Class name", Black_c, true);
        create_Field(this, "Allergy_class_name_field", GU::Point(10,340), GU::Size(100,20), " ", Black_c, true);
        create_button(this, "Allergy_search", GU::Point(120, 340), GU::Size(50,20), Symbol("Search"), true, screen_ptr, true, Gray_c, LightGray_c);
        buttons[Symbol("Allergy_search")]->set_property("Name", "Search");
        
    }

    
    
    
}



void TLD_device::create_medications_display(bool first_screen, bool second_screen, bool second_screen_searchResults, bool second_screen_formSelection){
    
    
    Trace_out << " Does it come here ? ............................................   mmm    "<< current_pointed_to_object_name <<  endl;

    if(first_screen == true) {
        
        create_button(this, "Medications_prescribe", GU::Point(100, 100), GU::Size(50, 20), "Prescribe", true, screen_ptr, true, DarkGray_c, LightGray_c);
        buttons[Symbol("Medications_prescribe")]->set_property("Name", "Prescribe");
        
        create_label(this, "Medications_label_scheduled", GU::Point(100, 200), GU::Size
                     (50, 20), "Scheduled", Black_c, true);
        create_label(this, "Medications_label_PRN", GU::Point(200, 200), GU::Size(50, 20), "PRN", Black_c, true);
        
        
    }
    
    if ( second_screen == true) {
    
        create_label(this, "Medications_label_name", GU::Point(10, 200), GU::Size(50, 20), "Name", Black_c, true);
        
        create_Field(this, "Medications_searchDrug", GU::Point(100, 200), GU::Size(50, 20), " ", Black_c, true);
        
        create_button(this, "Medications_searchButton", GU::Point(200, 200), GU::Size(50, 20), "Search", true, screen_ptr, true, DarkGray_c, LightGray_c);
        buttons[Symbol("Medications_searchButton")]->set_property("Name", "Search");
        
    }
    
    if (second_screen_searchResults == true) {
        Trace_out << " This is the place that it comes down to " << endl; 
        
        fields_t::iterator it_fields = fields.find(Symbol("Medications_searchDrug"));
      
      //  Trace_out << (it_fields->first) << " is the search result" << endl;
        
        Symbol screenName = (it_fields->second)->get_string();
        
       // Trace_out << screenName << " is the screen name " << endl;
        create_button(this, "Medications_searchResults", GU::Point(100, 300), GU::Size(50, 20), screenName, true, screen_ptr, true, Blue_c, Red_c);
        
        buttons[Symbol("Medications_searchResults")]->set_property("Name", screenName);
        //scheduledMedications[screenName] = "";
    }
    
    if (second_screen_formSelection == true) {
        Trace_out << " Hello world! " << endl;
    }
    
    
}






// If the user points to an object, remember what was pointed-to,
// and modify the display accordingly.
void TLD_device::handle_Point_event(const Symbol& target_name)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Point_to: " << target_name << endl;
	if (target_name == "Absent" || target_name == Nil_c)
		Normal_out << processor_info() << " Point_to non-existent object: " << target_name << endl;
	current_pointed_to_object_name = target_name;
	Smart_Pointer<Widget> ptr = screen_ptr->get_widget_ptr(current_pointed_to_object_name);
    
    // Trace_out << " Does it come here ? ............................................       "<< current_pointed_to_object_name <<  endl;
    
    /*  if(!ptr){
     ptr = screen_ptr->get_widget_ptr(LabeledField1_c);
     
     //    Trace_out << " Does it come here ? ............................................       "<< current_pointed_to_object_name.str() << " ........ "  <<  endl;
     
     }
     */
	if(ptr) {
		GU::Point new_loc = ptr->get_location();
		cursor_ptr->set_location(new_loc);
        

        
    }
	
	set_visual_object_property(Cursor_name_c, Pointing_to_c, target_name);
	output_display();
}





void TLD_device::handle_Click_event(const Symbol& button_name)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Click: " << button_name
		<< " on " << current_pointed_to_object_name << endl;
	
    //	if(blip_ptr && current_pointed_to_object_name == blip_ptr->get_name() && blip_ptr->get_property() == Red_c) {
    /* 	if(blip_ptr && current_pointed_to_object_name == blip_ptr->get_name()) {
     blip_ptr->set_property(Color_c, Green_c);
     // make the blip disappear a tenth second from now
     schedule_delay_event(100, "Blip_disappear", "");
     output_display();
     return;
     } */
    //	else {
    buttons_t::iterator it_buttons = buttons.find(current_pointed_to_object_name);
    //labeledFields_t::iterator it_labeledFields = labeledFields.find(current_pointed_to_object_name);
    
    fields_t::iterator it_fields = fields.find(current_pointed_to_object_name);
    
  //  scheduledMedications_t::iterator it_scheduledMedications = scheduledMedications.find(current_pointed_to_object_name);
    
    if(it_buttons == buttons.end() && it_fields == fields.end())
        throw Device_exception(this, "Click-on unrecognized object");
    
    if(it_buttons != buttons.end()){
        Smart_Pointer<Button_widget> current_button_ptr = it_buttons->second;
        
        if(!(current_button_ptr->get_state()))
            throw Device_exception(this, "Click-on button that is off");
        
        current_button_ptr->set_state(false);
        
        
        
        if(current_pointed_to_object_name == "Button_allergy"){
            Trace_out << processor_info() << " Now new screen for entering allergy information " << endl;
            
            clear_objects_on_screen();
            
            
            create_allergies_display(true, false, false);
        }
        if( current_pointed_to_object_name == "Allergy_option3") {
            Trace_out << processor_info() << " ...........LLLL ......  .. " << endl;
            Main_screen_string1 << "Class allergy to"; 
            create_allergies_display(false, true, false);
        } 

        if( current_pointed_to_object_name == "Allergy_option4") {
            Trace_out << processor_info() << " ...........LLLL ......  .. " << endl;
            Main_screen_string1 << "Drug allergy to"; 
            create_allergies_display(false, true, false);
        }
        
        
        if(current_pointed_to_object_name == "Allergy_option6"){
            
            Trace_out << processor_info() << " Now new screen for entering allergy information 2 " << endl;
            
            Main_screen_string1 << "Non-drug allergy to"; 
            
            create_allergies_display(false, false, true);
            
        }
        
        if(current_pointed_to_object_name == "Allergy_continue"){
            clear_objects_on_screen();
            create_homeScreen_display(true, true);
            
        }
        
        if(current_pointed_to_object_name == "Allergy_search"){
            create_allergies_display(false, true, true);
            
        }
        
        if(current_pointed_to_object_name == ButtonMedications_c){
            clear_objects_on_screen();
            create_medications_display(true, false, false, false);
        }
        
        if(current_pointed_to_object_name == "Medications_prescribe"){
            clear_objects_on_screen(); 
            create_medications_display(false, true, false, false);
            
        }
        
        if(current_pointed_to_object_name == "Medications_searchButton") {
            
            create_medications_display(false, false, true, false);
            
        
        }
        
        if(current_pointed_to_object_name == "Medications_searchResults"){
            Trace_out << " Time to create scheduled medications entery form !!! " << endl;
            
            create_medications_display(false, false, false, true);
            
        }
        
        
        
        
    }
    else if(it_fields != fields.end()){
        if(current_pointed_to_object_name == Field_AllergySubstance_c){
            // Trace_out << " blah blah blah blah blah blah blah blah blah blah blabh blah blah " << endl;
            
        }
        
    }
	
    
	output_display();
}








void TLD_device::handle_Type_In_event(const Symbol& type_in_string){
    
    // can set current clicked on object name, and make sure it and current pointed to object name are the same
    fields_t::iterator it_fields = fields.find(current_pointed_to_object_name);
    (it_fields->second)->set_string(type_in_string);
    
    Main_screen_string1 << " " << type_in_string;
    
    
    if(current_pointed_to_object_name == Field_AllergySubstance_c) {
        
        
        it_fields = fields.find(Field_AllergyComments_c);
        (it_fields->second)->set_string(" ");
        
        
        // change color or next fields here;  maybe update it in the screen_ptr
    }
    
    output_display();
}




// the device delay event is either to make a blip appear, or disappear and either
// schedule a new appearance or stop the simulation
/* void TLD_device::handle_Delay_event(const Symbol& type, const Symbol& datum,
                                    const Symbol& object_name, const Symbol& property_name, const Symbol& property_value)
{
	if(Trace_out && get_trace())
		Trace_out << processor_info() << " Delay event: "
		<< type << ' ' << datum << ' '
		<< object_name << ' ' << property_name << ' '  << property_value
		<< endl;
	
	if(type == "Blip_appear") {
		ostringstream oss;
		oss << "Blip" << blip_counter;
		string blip_name = oss.str();
		blip_ptr = new Object_widget(this, Symbol(blip_name), GU::Point(240, 200 + 50 * blip_counter), GU::Size(20, 20));
		blip_ptr->set_property(Shape_c, Triangle_c);
		blip_ptr->set_property(Color_c, Red_c);
		screen_ptr->add_widget(blip_ptr);
		blip_ptr->present();
    }
	else if(type == "Blip_disappear") {
		if(!blip_ptr)
			throw Device_exception(this, "Event to disappear nonexisting Blip received");
		if(current_pointed_to_object_name == blip_ptr->get_name())
			current_pointed_to_object_name = Nil_c;
		screen_ptr->remove_widget(blip_ptr);
		blip_ptr = 0;
		blip_counter++;
		// stop the simulation if that was the third click-on
		if(blip_counter > 3) {
			stop_simulation();
			return;
        }
		// schedule a new appearance sometime between 1 sec and 2 secs from now
		int delay = 1000 + random_int(2000);
		schedule_delay_event(delay, "Blip_appear", "");
    }
	else {
		throw Device_exception(this, "Unrecognized Delay event received");
    }
	
	output_display();
}*/ 

