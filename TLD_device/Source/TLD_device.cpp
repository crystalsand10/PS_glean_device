#include "TLD_device.h"
#include "GLEANKernel/Device_exception.h"
#include "GLEANKernel/Glean_standard_symbols.h"
#include "GLEANKernel/Output_tee_globals.h"
#include "GLEANKernel/Utility_templates.h"	// for Delete_second
#include "GLEANKernel/Numeric_utilities.h"	// for random_int
#include "GLEANKernel/Symbol_utilities.h"	// for concatenate_to_Symbol
#include <iostream>
#include <iomanip>
#include <regex> 

using namespace std;

// _d for defined
/* In the display of the allergy screens */
const Symbol ButtonAllergy_b("Button_allergy");
const Symbol ButtonMedications_d("Button_medications");
const Symbol Field_AllergySubstance_d("AllergySubstance");
const Symbol Field_AllergyComments_d("AllergyComments");
const Symbol Allergy_Polygon1_d("Allergy_Polygon1");
const Symbol Allergy_Polygon2_d("Allergy_polygon2");
const Symbol Allergy_continue_b("Allergy_continue");
const Symbol Allergy_cancel_d("Allergy_cancel");
const Symbol Allergy_Polygon3_d("Allergy_Polygon3");
const Symbol Allergy_option1_b("Allergy_option1_d");
const Symbol Allergy_option2_b("Allergy_option2_d");
const Symbol Allergy_option3_b("Allergy_option3_d");
const Symbol Allergy_option4_b("Allergy_option4_d");
const Symbol Allergy_option5_b("Allergy_option5_d");
const Symbol Allergy_option6_b("Allergy_option6_d");

/* In the display of the medications screens */
const Symbol Medications_prescribe_b("Medications_prescribe");
const Symbol Medications_update_b("Medications_update");
const Symbol Medications_close_b("Medications_close");
const Symbol Medications_searchDrug_lF("Medications_searchDrug");
const Symbol Medications_searchButton_b("Medications_searchButton");
const Symbol Medications_searchResults_b("Medications_searchResults");
const Symbol Medications_label_ChartUpdates_l("Medications_label_ChartUpdates");
const Symbol Medications_label_scheduled_l("Medications_label_scheduled");
const Symbol Medications_label_PRN_l("Medications_label_PRN");
const Symbol Medications_third_scheduled_b("Medications_third_scheduled");
const Symbol Medications_third_frequency_l("Medications_third_frequency");
const Symbol Medications_third_PRN_b("Medications_third_PRN");
const Symbol Medications_third_stat_b("medications_third_stat");
const Symbol Medications_continue_b("Medications_continue");
const Symbol Medications_third_minInterval_options_b("Medications_third_minInterval_options");
const Symbol Medications_third_maxOf_options_b("Medications_third_maxOf_options");
const Symbol Medications_third_maxOf_TimeOptions_b("Medications_third_maxOf_TimeOptions"); 
const Symbol Medications_minInterval_options_1_mI("Medications_minInterval_options_1");
const Symbol Medications_minInterval_options_2_mI("Medications_minInterval_options_2");
const Symbol Medications_minInterval_options_3_mI("Medications_minInterval_options_3");
const Symbol Medications_strength_b("Medications_strength");
const Symbol Medications_third_dose_lF("Medications_third_dose");
const Symbol Medications_third_minInterval_lF("Medications_third_minInterval");
const Symbol Medications_third_maxOf_lF("Medications_third_maxOf");
const Symbol Medications_third_maxOf_per_lF("Medications_third_maxOf_per"); 
const Symbol Medications_form_b("Medications_form");
const Symbol Medications_maxOf_options_1_mI("Medications_maxOf_menuOption1");
const Symbol Medications_maxOf_options_2_mI("Medications_maxOf_menuOption2");
const Symbol Medications_warning_override_b("Medications_warning_override");
const Symbol Medications_third_qualifier_lf("Medications_third_qualifier"); 
const Symbol Medications_warning_continue_b("Medications_warning_continue"); 
const Symbol Medications_warning_heading_o("Medications_warning_heading");
const Symbol Medications_warning_pregnancy_l("Medications_warning_pregnancy");

const Symbol Medications_cease_scheduled_b("Medications_cease_scheduled");
// const Symbol Medications_toCeaseMed_l("Medications_toCeaseMed");
const Symbol Medications_toCeaseMed_b("Medications_toCeaseMed");


const Symbol Medications_sch_for_lf("Medications_sch_for");
const Symbol Medications_sch_route_l("Medications_sch_route");
const Symbol Medications_sch_schedule_l("Medications_sch_schedule");


const Symbol Medications_sch_forOptions_b("Medications_sch_forOptions");
const Symbol Medications_sch_schOptions_b("Medications_sch_schOptions");
const Symbol Medications_sch_routeOptions_b("Medications_sch_routeOptions");

const Symbol Medications_schFor_options1_mI("Medications_schFor_options1");

const Symbol Medications_schSch_options1_mI("Medications_schSch_options1");
const Symbol Medications_schSch_options2_mI("Medications_schSch_options2");
const Symbol Medications_schSch_options3_mI("Medications_schSch_options3");

const Symbol Medications_schRoute_options1_mI("Medications_schRoute_options1"); 



/* Medication Names */
const Symbol PRN_paracetamol("Paracetamol");
const Symbol PRN_panadeine("Panadeine");
const Symbol PRN_oxycodone("Oxycodone");

const Symbol Scheduled_norfloxacin("Norfloxacin");
const Symbol Scheduled_thiamine("Thiamine");
const Symbol Scheduled_chloramphenicol("Chloramphenicol");

const Symbol Replace_P_prednisolone("Prednisolone");
const Symbol Replace_P_heparin("Heparin Sodium"); // Check this works ;; changed from Heparin ;; 
const Symbol Replace_P_sotalol("Sotalol");

const Symbol Replace_C_hydrocortisone("Hydrocortisone"); 
const Symbol Replace_C_benzylpenicillin("Benzylpenicillin"); 
const Symbol Replace_C_amiodarone("Amiodarone");






/* Strings to print out, this one is for allergy // change name */ 
std::stringstream Main_screen_string1;
// std::stringstream Main_screen_string2;


int initial_prescribed_medication_location;

TLD_device::TLD_device(const std::string& id, Output_tee& ot) :
Device_base(id, ot)
{
	// If one object property refers to another object, GLEAN currently requires that
	// the physical name be same as local names ...
	Widget::set_add_widget_type_property(true);  // e.g. so "Type Button" will be a property of an object
}



void TLD_device::initialize()
{
    clear_objects_on_screen();


	Device_base::initialize();
	clear_display();	
	current_pointed_to_object_name = Nil_c;
    current_searched_medication = Nil_c;
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
    create_homeScreen_display(false);
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
    // Trace_out<< " name of the button is " << name << " pppppppppppppppppppppppppppppppp " << endl;
	buttons[name] = ptr;
	
    
    screen_ptr->add_widget(ptr);
    
    
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

    fields[widget_name] = ptr;
    screen_ptr->add_widget(ptr);

    if(should_present == true){
        ptr->present();
    }
    
}

void TLD_device::create_labeledField(Device_base * dev_ptr, const Symbol& widget_name,GU::Point location, GU::Size label_size, GU::Size field_size, const Symbol& label, bool should_present, bool is_dosageFields){
    Smart_Pointer<Labeled_field_widget> ptr = new Labeled_field_widget(dev_ptr, widget_name, location, label_size, field_size, label);
    
    // ptr->set_add_widget_type_property(true);
    // ptr->set_string("blah");
    labeledFields[widget_name] = ptr;
    screen_ptr->add_widget(ptr);
    //  screen_ptr->present_property("Name", "Substances");
    //   Trace_out << " name is .......... " << ptr->get_name() << " .......... " << endl;
    if(is_dosageFields == true) {
        assign_initial_dosage(ptr);
    }
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
/*	device_out << "--------------------\n" << processor_info() << " Display: " << endl;
	for(buttons_t::const_iterator it = buttons.begin(); it != buttons.end(); ++it) {
		device_out << (it->second)->get_name()
		<< ' ' << (it->second)->get_location() << ' ' << (it->second)->get_state() << endl;
    }
    
	device_out << "Cursor pointing at " << current_pointed_to_object_name << endl;
	device_out << "--------------------" << endl;;	*/ 
}




/* This function clears everything including the screen and the cursor */
void TLD_device::clear_display()
{
    clear_objects_on_screen();

    screen_ptr = 0;
    cursor_ptr = 0;
    
    buttons.clear();
    fields.clear();
    
    labels.clear();
    objects.clear();
    polygons.clear();
    labeledFields.clear();
    menuItems.clear();
    windows.clear();
 
    scheduledMedications.clear();
    prnMedications.clear();
  //  current_searched_medication = nullptr;
  //  current_pointed_to_object_name = nullptr;
    Main_screen_string1.str(""); 
    Main_screen_string1.clear();

}
    


/* For each of the various objects on the screen,
 * depresents them on the screen; 
 * removes them from the screen_ptr; 
 * finally clears them from its respective type array
 * This is better for clearing objects without also clearing the screen and the cursor. 
 */
void TLD_device::clear_objects_on_screen(){
    
    for(buttons_t::const_iterator it = menuItems.begin(); it != menuItems.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second));
    }
    
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
    
    for(windows_t::const_iterator it = windows.begin(); it != windows.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second));
    }

    for(labeledFields_t::const_iterator it = labeledFields.begin(); it != labeledFields.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second));
    }

    

    
    
    buttons.clear();
    fields.clear();
    polygons.clear();
    objects.clear();
    labels.clear();
    labeledFields.clear();
    windows.clear();
}






/* Creates the home screen from which the user can click on various options to add allergies, medication etc. */
void TLD_device::create_homeScreen_display(bool first_display){
 /*   Smart_Pointer<Object_widget> ptr = new Object_widget(this, "test", GU::Point(50, 50), GU::Size(40, 40));
    ptr->set_property(Type_c, Menu_c);
    ptr->set_property(Shape_c, Rectangle_c);
    ptr->set_property(Text_c, "  ");
    ptr->set_property("totalMenuItems", 3);
    ptr->set_property("Menu_item1", "Morning");
    ptr->set_property("Menu_item2", "Afternoon");
    ptr->set_property("Menu_item3", "Evening");
    ptr->set_property(Color_c, White_c);
    
    screen_ptr->add_widget(ptr);
    */
  //  ptr->set_property(Position_c, )
    
/*
     Smart_Pointer<Container_widget> ptr = new Window_widget(this, "Test_container", GU::Point(50, 50), GU::Size(40, 40), "Test_window");
    
    Smart_Pointer<Button_widget> button_ptr = new Button_widget(this, "Test_button", GU::Point(50, 50), GU::Size(40, 10), "Test", Gray_c, LightBlue_c, true);
    
	//buttons["Test_button"] = button_ptr;
	screen_ptr->add_widget(button_ptr);
    
 //   ptr->add_widget(button_ptr);
    screen_ptr->add_widget(ptr);
    
    */ 
    
 //   ptr->set_property(Menu_item_c, "item1");
 //   ptr->set_property(Menu_item_c, "item2");
    

    
    
    create_button(this, ButtonAllergy_b, GU::Point(500, 200), GU::Size(35, 20),  "Add", true, screen_ptr, first_display, LightGray_c, LightGray_c);
    buttons[ButtonAllergy_b]->set_property("Name", "Add");
    buttons[ButtonAllergy_b]->set_property(Shape_c, Rectangle_c);
    
    Trace_out<< " name of the button is : v2  :: " << buttons[ButtonAllergy_b]->get_name().str() << " pppppppppppppppppppppppppppppppp " << endl;
	
    
    create_button(this, Symbol("button2"),  GU::Point(10, 230), GU::Size(230, 20), "Medications on admission", false, screen_ptr , first_display, Gray_c, LightGray_c);
    buttons["button2"]->set_property("Shape", Rectangle_c);
    
    
    create_button(this, Symbol("button3"),  GU::Point(10, 270), GU::Size(230, 20), "Chart history", false, screen_ptr , first_display, Gray_c, LightGray_c);
    buttons["button3"]->set_property("Shape", Rectangle_c);
    
    
    create_button(this, Symbol("button4"),  GU::Point(10, 300), GU::Size(230, 20), "Script history", false, screen_ptr , first_display, Gray_c, LightGray_c);
    buttons["button4"]->set_property("Shape", Rectangle_c);
    
    
    create_button(this, Symbol("button5"),  GU::Point(10, 330), GU::Size(230, 20), "Patient history", false, screen_ptr , first_display, Gray_c, LightGray_c);
    buttons["button5"]->set_property("Shape", Rectangle_c);
    
    
    
    create_button(this, ButtonMedications_d, GU::Point(10, 200), GU::Size(230, 20),  "Medications", true, screen_ptr, first_display, LightGray_c, LightGray_c);
    buttons[ButtonMedications_d]->set_property("Name", "Medications");
    buttons[ButtonMedications_d]->set_property("Shape", Rectangle_c);
    
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
    
    
    if(Main_screen_string1.rdbuf()->in_avail() != 0) {
        // create a label perhaps and display Main_screen_string1
        
        create_label(this, "Allergy_information string", GU::Point(258, 230), GU::Size(100, 20), Main_screen_string1.str(), Red_c, true);
    }
    
    display_prescribedMedications();
    
    
}


void TLD_device::display_prescribedMedications(){
    
    initial_prescribed_medication_location = 400;
    
    bool flag_hasScheduled = false;
    bool flag_hasPRN = false; 
    
    for(scheduledMedications_t::const_iterator it = scheduledMedications.begin(); it != scheduledMedications.end(); ++it) {
        if(flag_hasScheduled == false) {
            
            initial_prescribed_medication_location = initial_prescribed_medication_location + 10;
            
            create_label(this, "Subheader_scheduled", GU::Point(258, initial_prescribed_medication_location), GU::Size(200, 10), "Scheduled Medications", Black_c, true);
            
            initial_prescribed_medication_location = initial_prescribed_medication_location + 20; 
            
            flag_hasScheduled= true;
        } 
        const std::string widget_name = std::string("Medications_") + (it->first).str();
        
        std::string medication_display = (it->first).str() + (it->second).str();
        
        create_Object(this, widget_name, GU::Point(258, initial_prescribed_medication_location), GU::Size(200, 10), White_c, Rectangle_c, medication_display, true);
        Trace_out << "Scheduled medications names : " << (it->first) << endl;
        
        initial_prescribed_medication_location = initial_prescribed_medication_location + 20;
        
    }
    
    for(prnMedications_t::const_iterator it = prnMedications.begin(); it != prnMedications.end(); ++it){
        if(flag_hasPRN == false) {
            
            initial_prescribed_medication_location = initial_prescribed_medication_location + 10;
            
            
            create_label(this, "Subheader_PRN", GU::Point(258, initial_prescribed_medication_location), GU::Size(200, 10), "PRN Medications", Black_c, true);
            
            flag_hasPRN = true;
            initial_prescribed_medication_location = initial_prescribed_medication_location + 20;
            
        }
        
        
        const std::string widget_name = std::string("Medications_") + (it->first).str();
        
        std::string medication_display = (it->first).str() + (it->second).str();
        
        create_Object(this, widget_name, GU::Point(258, initial_prescribed_medication_location), GU::Size(200, 10), White_c, Rectangle_c, medication_display, true);
        
        
        
        Trace_out << "PRN medications names : " << (it->first) << endl;
        
        initial_prescribed_medication_location = initial_prescribed_medication_location + 20; 
        
    }
}


/* Creates the display when user clicks on Add. Second screen appears when the user clicks on a selection, in this case - non-drug allergy */
void TLD_device::create_allergies_display(bool isFirstScreen, bool isOption3, bool isOption6){

    
    if( isFirstScreen == true) {

        std::vector<GU::Point> in_vertices1 {GU::Point(1, 190), GU::Point(650 , 190)}; // horizontal line
        create_polygon(this, Allergy_Polygon1_d, in_vertices1, DarkGray_c, true);
        
        std::vector<GU::Point> in_vertices2 {GU::Point(318, 190), GU::Point( 318, 550)}; // vertical line
        create_polygon(this, Allergy_Polygon2_d, in_vertices2, DarkGray_c, true);
    
        create_button(this, Allergy_continue_b, GU::Point(20, 130), GU::Size(50, 15), "Continue", true, screen_ptr, true, Gray_c, LightGray_c);
        buttons[Allergy_continue_b]->set_property("Name", "Continue");
        
        create_button(this, Allergy_cancel_d, GU::Point(100, 130), GU::Size(50, 15), "Cancel", false, screen_ptr, true, Gray_c, LightGray_c);
        
        std::vector<GU::Point> in_vertices3 {GU::Point(1, 150), GU::Point(650 , 150)}; // horizontal line
        create_polygon(this, Allergy_Polygon3_d, in_vertices3, DarkGray_c, true);
        
        
        create_button(this, Allergy_option1_b, GU::Point(10, 200), GU::Size(10, 10), "  Patients Allergy Status", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, Allergy_option2_b, GU::Point(10, 220), GU::Size(10, 10), "  No known allergies or intolerances", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, Allergy_option3_b, GU::Point(10, 240), GU::Size(10, 10), "  Class allergy", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, Allergy_option4_b, GU::Point(10, 260), GU::Size(10, 10), "  Drug allergy", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, Allergy_option5_b, GU::Point(10, 280), GU::Size(10, 10), "  Drug intolerances", true, screen_ptr, true, LightGray_c, Black_c);
        create_button(this, Allergy_option6_b, GU::Point(10, 300), GU::Size(10, 10), "  Non drug allergy", true, screen_ptr, true, LightGray_c, Black_c);
        
        buttons[Allergy_option3_b]->set_property("Name", "ClassAllergy");
        buttons[Allergy_option4_b]->set_property("Name", "DrugAllergy");
        buttons[Allergy_option6_b]->set_property("Name", "NonDrug");
    }

    if( isOption6 == true && isOption3 == true) {
    
        create_label(this, "Allergy_label_Comments", GU::Point(350, 400), GU::Size(100, 20), "Comments", Green_c, true);
        
        create_Field(this, Field_AllergyComments_d, GU::Point(420, 400), GU::Size(100, 50), " ", Black_c, true);
        
        
        return; 
    }
    
    
    if( isOption6 == true){
        
        Trace_out << processor_info() << " Now new screen for entering allergy information 2.1 " << endl;
        
        create_label(this, "Allergy_label_Substance", GU::Point(340, 230), GU::Size(100, 20), "Substance", Red_c, true);
        create_label(this, "Allergy_label_Comments", GU::Point(350, 400), GU::Size(100, 20), "Comments", Green_c, true);
        create_Field(this, Field_AllergySubstance_d, GU::Point(420, 230),  GU::Size(100, 20)," ", Black_c, true);
        create_Field(this, Field_AllergyComments_d, GU::Point(420, 400), GU::Size(100, 50), "", Black_c, true);
        
    }
    
    if( isOption3 == true){
        /* label, field, button
            clicks on search, choses, enters comments rash.
        */
        create_label(this, "Allergy_class_name", GU::Point(10, 320), GU::Size(30, 30), "Class name", Black_c, true);
        create_Field(this, "Allergy_class_name_field", GU::Point(10,340), GU::Size(100,20), " ", Black_c, true);
        create_button(this, "Allergy_search", GU::Point(120, 340), GU::Size(50,20), Symbol("Search"), true, screen_ptr, true, Gray_c, LightGray_c);
        buttons[Symbol("Allergy_search")]->set_property("Name", "Search");
        
    }

    
    
    
}



void TLD_device::create_medications_display(bool first_screen, bool second_screen, bool second_screen_searchResults, bool second_screen_formSelection, bool third_screen, bool third_screen_PRN, bool fourth_screen){
    
    
    Trace_out << " Medications_display."<< current_pointed_to_object_name <<  endl;

    if(first_screen == true) {
        
        create_button(this, Medications_prescribe_b, GU::Point(160, 150), GU::Size(80, 15), "Prescribe", true, screen_ptr, true, DarkGray_c, LightGray_c);
        buttons[Medications_prescribe_b]->set_property("Name", "Prescribe");
        buttons[Medications_prescribe_b]->set_property(Shape_c, Rectangle_c);
        
        
       // create_label(this, Medications_label_ChartUpdates_l, GU::Point(30, 140), GU::Size(50, 20), "Chart Updates", Black_c, true);
        
       // create_label(this, Medications_label_scheduled_l, GU::Point(150, 180), GU::Size
       //              (50, 20), "Scheduled", Black_c, true);
       // create_label(this, Medications_label_PRN_l, GU::Point(240, 180), GU::Size(50, 20), "PRN", Black_c, true);
       
        
        create_button(this, Medications_update_b, GU::Point(15, 150), GU::Size(50, 15), "Update", true, screen_ptr, true, DarkGray_c, LightGray_c);
        buttons[Medications_update_b]->set_property("Name", "Update");
        buttons[Medications_update_b]->set_property(Shape_c, Rectangle_c);
        
        
        create_button(this, Medications_close_b, GU::Point(10, 90), GU::Size(50, 15), "Close", true, screen_ptr, true, White_c, White_c);
        buttons[Medications_close_b]->set_property("Name", "Close");
        buttons[Medications_close_b]->set_property(Shape_c, Rectangle_c);
        
        
        std::vector<GU::Point> in_vertices1 {GU::Point(1, 85), GU::Point(650 , 85)}; // horizontal line
        create_polygon(this, Symbol("Polygon1"), in_vertices1, DarkGray_c, true);
        
        
        std::vector<GU::Point> in_vertices2 {GU::Point(1, 104), GU::Point(650 , 104)}; // horizontal line
        create_polygon(this, Symbol("Polygon2"), in_vertices2, DarkGray_c, true);
        
        
    }
    
    if ( second_screen == true) {
    
  //      create_label(this, "Medications_label_name", GU::Point(10, 200), GU::Size(50, 20), "Name", Black_c, true);
        
  //      create_Field(this, "Medications_searchDrug", GU::Point(100, 200), GU::Size(50, 20), " ", Black_c, true);
        

        create_labeledField(this, Medications_searchDrug_lF, GU::Point(10,200), GU::Size(38,20), GU::Size(150, 20), "Name", true, false);
        
        create_button(this, Medications_searchButton_b, GU::Point(208, 200), GU::Size(50, 15), "Search", true, screen_ptr, true, DarkGray_c, Gray_c);
        buttons[Medications_searchButton_b]->set_property("Name", "Search");
        buttons[Medications_searchButton_b]->set_property(Shape_c, Rectangle_c);
        
        
        std::vector<GU::Point> in_vertices2 {GU::Point(262, 190), GU::Point( 262, 550)}; // vertical line
        create_polygon(this, Symbol("Polygon1"), in_vertices2, LightBlue_c, true);
        
    }
    
    if (second_screen_searchResults == true) {
        
        
        if(iequals(current_searched_medication.str(), "Heparin")){
            current_searched_medication = "Heparin Sodium"; 
        }
        
           create_button(this, Medications_searchResults_b, GU::Point(25, 230), GU::Size(225, 20), current_searched_medication, true, screen_ptr, true, Yellow_c, Yellow_c);
        
        buttons[Medications_searchResults_b]->set_property("Name", current_searched_medication);
        buttons[Medications_searchResults_b]->set_property(Shape_c, Rectangle_c);
        
        
        scheduledMedications[current_searched_medication] = "";
        
    }
    
    if (second_screen_formSelection == true) {
        display_medications_FormStrength(current_searched_medication.str());
    }
    
    if (third_screen == true) {
        clear_objects_on_screen();
        
        create_label(this, Medications_third_frequency_l , GU::Point(180, 240), GU::Size(50, 10), "Frequency", Black_c, true);
    
        create_button(this, Medications_third_scheduled_b, GU::Point(270, 240), GU::Size(10, 10), "  Scheduled", true, screen_ptr, true, Black_c, Gray_c);
       // buttons["medications_third_scheduled"]->set_property("Name", "Scheduled");
        
        
        
        
        create_button(this, Medications_third_PRN_b, GU::Point(380, 240), GU::Size(10, 10), "  PRN", true, screen_ptr, true, Gray_c, Black_c);
        buttons[Medications_third_PRN_b]->set_property("Name", "PRN");
        
        create_button(this, Medications_third_stat_b, GU::Point(450, 240), GU::Size(10, 10), "  Stat", true, screen_ptr, true, Gray_c, Gray_c);
        
        
        create_labeledField(this, Medications_third_dose_lF, GU::Point(220, 180), GU::Size(50, 12), GU::Size(50, 10), "Dose", true, true);
        
        
        create_label(this, Medications_sch_route_l, GU::Point(300, 300), GU::Size(50, 12), "Route", Black_c, true);
        
        create_label(this, Medications_sch_schedule_l, GU::Point(280, 400) , GU::Size(50, 12), "Schedule", Black_c, true);
        
 
        create_button(this, Medications_sch_routeOptions_b, GU::Point(327, 300), GU::Size(74,15), "", true, screen_ptr, true, Gray_c, Gray_c);
        buttons[Medications_sch_routeOptions_b]->set_property("Name", "RouteOptions");
        buttons[Medications_sch_routeOptions_b]->set_property(Shape_c, Rectangle_c);
        
        create_button(this, Medications_sch_schOptions_b, GU::Point(327, 400), GU::Size(74,15), "", true, screen_ptr, true, Gray_c, Gray_c);
        buttons[Medications_sch_schOptions_b]->set_property("Name", "ScheduleOptions");
        buttons[Medications_sch_schOptions_b]->set_property(Shape_c, Rectangle_c); 
        
        
        
        create_labeledField(this, Medications_sch_for_lf, GU::Point(59, 500), GU::Size(210, 15), GU::Size(50, 15), "For", true, false);
        
        
        create_button(this, Medications_sch_forOptions_b, GU::Point(327, 500), GU::Size(74,15), "", true, screen_ptr, true, Gray_c, Gray_c);
        buttons[Medications_sch_forOptions_b]->set_property("Name", "ForOptions");
        buttons[Medications_sch_forOptions_b]->set_property(Shape_c, Rectangle_c);
        
        
        
        create_button(this, Medications_continue_b, GU::Point(12, 90), GU::Size(75, 12), "Continue", true, screen_ptr, true, LightGray_c, LightGray_c);
        buttons[Medications_continue_b]->set_property("Name", "Continue");
        buttons[Medications_continue_b]->set_property(Shape_c, Rectangle_c);
        
        
        
    }
    
    if( third_screen_PRN == true) {
        create_labeledField(this, Medications_third_minInterval_lF, GU::Point(59, 500), GU::Size(210, 15), GU::Size(50, 15), "Minimum Dosage Interval", true, false);
        
        create_button(this, Medications_third_minInterval_options_b, GU::Point(327, 500), GU::Size(74,15), " ", true, screen_ptr, true, Gray_c, Gray_c);
        buttons[Medications_third_minInterval_options_b]->set_property("Name", "MinIntervalOptions");
        buttons[Medications_third_minInterval_options_b]->set_property(Shape_c, Rectangle_c);
        
        

        
        create_labeledField(this, Medications_third_maxOf_lF, GU::Point(164, 530), GU::Size(104, 15), GU::Size(50, 15), "Maximum of", true, false);
        create_button(this, Medications_third_maxOf_options_b, GU::Point(327, 530), GU::Size(50,15), "  ", true, screen_ptr, true, Gray_c, Gray_c);
        buttons[Medications_third_maxOf_options_b]->set_property("Name", "MaxOfOptions");
        buttons[Medications_third_maxOf_options_b]->set_property(Shape_c, Rectangle_c);
        
        
        
        create_labeledField(this, Medications_third_maxOf_per_lF, GU::Point(390, 530), GU::Size(35, 15), GU::Size(50, 15), "per", true, false);
        // create_label(this, "Medications_third_maxOf_per", , , "per", Black_c, true);
        // create_Field(this, "Medications_third_maxOfTime", GU::Point(450, 530), , "", Black_c, true);
        create_button(this, Medications_third_maxOf_TimeOptions_b, GU::Point(490, 530), GU::Size(50,15), "  ", true, screen_ptr, true, Gray_c, Gray_c);
        buttons[Medications_third_maxOf_TimeOptions_b]->set_property(Shape_c, Rectangle_c); 
        
        
        create_labeledField(this, Medications_third_qualifier_lf, GU::Point(172, 560), GU::Size(94, 15), GU::Size(300, 15), "Qualifier", true, false);
    }
    
    if(fourth_screen == true) {

        for(scheduledMedications_t::const_iterator it = scheduledMedications.begin(); it != scheduledMedications.end(); ++it) {
            
            if((it->first) == current_searched_medication){
                const std::string widget_name = std::string("Medications_") + (it->first).str();
                
                std::string medication_display = (it->first).str() + (it->second).str();
                
                create_Object(this, widget_name, GU::Point(55, 180), GU::Size(200, 10), White_c, Rectangle_c, medication_display, true);
                
                
                
                Trace_out << "Scheduled medication to be updated : " << (it->first) << endl;
           
                
                
            }

            
            if ( iequals(current_searched_medication.str(), "Predisolone") || iequals(current_searched_medication.str(), "Heparin Sodium") || iequals(current_searched_medication.str(), "Sotalol") ){
                
                
                create_button(this, "Medications_Scheduled", GU::Point(100, 100), GU::Size(50, 10), "Scheduled", true, screen_ptr, true, Blue_c, Green_c);
            }
            
            
            
         //   std::string medication_display = (it->first).str() + (it->second).str();
            
        }
        
        for(prnMedications_t::const_iterator it = prnMedications.begin(); it != prnMedications.end(); ++it){
            
            if((it->first)== current_searched_medication){
                const std::string widget_name = std::string("Medications_") + (it->first).str();
                
                std::string medication_display = (it->first).str() + (it->second).str();
                
                create_Object(this, widget_name, GU::Point(55, 180), GU::Size(200, 10), White_c, Rectangle_c, medication_display, true);
                
                
                
                
                Trace_out << "PRN medication to be updated : " << (it->first) << endl;
                
            }
            

            
            
            
            
          //  std::string medication_display = (it->first).str() + (it->second).str();
            
            
        }
        
        
        
        
        

    }
}


void TLD_device::assign_initial_dosage(Smart_Pointer<Labeled_field_widget> ptr){
    
    //labeledFields_t::iterator it_labeledFields = labeledFields.find(current_searched_medication);
    
    //Trace_out << "Changing value of the dosage" << " ... ... " << endl;
    
    if(iequals(current_searched_medication.str(), PRN_paracetamol.str())){
        
        ptr->set_string("500");
        
        Trace_out << " paracetamol: " << " " << current_searched_medication.str() << " " << PRN_paracetamol.str() << endl;

    }
    
    if(iequals(current_searched_medication.str(), PRN_oxycodone.str())){
        
        ptr->set_string("5"); 
        
        Trace_out << " oxycodone " << endl;

    }

}


void TLD_device::display_medications_FormStrength(std::string medication){
    
    int label_pointX, label_pointY;
    int button_pointX, button_pointY;

    Symbol label_label;
    Symbol button_label;

    
    // PRN medication chart 1 
    if(iequals(medication, "Paracetamol")){
        
        label_pointX = 270; label_pointY = 520;
        button_pointX = 420; button_pointY = 520;
        
        label_label = "Tablet";
        button_label = "500mg";
        
        scheduledMedications[medication] = " (500 mg) Tablet";
        
    }
    
    // PRN medication chart 4
    if(iequals(medication, "panadeine")){
        label_pointX = 270; label_pointY = 550;
        button_pointX = 420; button_pointY = 550;
        
        label_label = "Tablet";
        button_label = "30mg + 500mg";
        
        scheduledMedications[medication] = " (30mg, 500mg) Tablet";
    }
    
    // PRN medication chart 5 
    if(iequals(medication, "Oxycodone")){
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Capsule";
        button_label = "5mg";
        
        scheduledMedications[medication] = " (5 mg) Capsule";
    }
    

    // Scheduled med 1
    if(iequals(medication, "Norfloxacin")){
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Tablet";
        button_label = "400mg";
        
        scheduledMedications[medication] = " (400mg) Tablet";
    }
    
    // Scheduled med 2
    if(iequals(medication, "Thiamine")){
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Tablet";
        button_label = "100mg";
        
        scheduledMedications[medication] = " (100mg) Tablet";
    }
    
    // Scheduled med 3
    if(iequals(medication, "Chloramphenicol")){
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Eye Drop";
        button_label = "0.5%";
        
        scheduledMedications[medication] = " (0.5%) Eye Drop";
    }
    
    
    // Replace P med 1
    if(iequals(medication, "Prednisolone")){
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Tablet";
        button_label = "25mg";
        
        scheduledMedications[medication] = " (25mg) Tablet";
    }
    
    // Replace P med 2
    if(iequals(medication, "Heparin Sodium")){ // Heparin Sodium 
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Injection";
        button_label = "5000units/0.2mL";
        
        scheduledMedications[medication] = " (5000units/0.2mL) Injection";
    }
    
    // Replace P med 3
    if(iequals(medication, "Sotalol")){
        label_pointX = 270; label_pointY = 500;
        button_pointX = 420; button_pointY = 500;
        
        label_label = "Tablet";
        button_label = "80mg";
        
        scheduledMedications[medication] = " (80mg) Tablet";
    }
    
    
    
    // Scheduled medication chart 1
    if(iequals(medication, "Cephazolin")){
        label_pointX = 270; label_pointY = 250;
        button_pointX = 420; button_pointY = 250;
        
        label_label = "Injection";
        button_label = "1g";
        
        scheduledMedications[medication] = " (1g) Injection";
        
        
    }
    
    
    create_label(this, Medications_form_b, GU::Point(label_pointX, label_pointY) , GU::Size(30, 10), label_label, Black_c, true);
    create_button(this, Medications_strength_b, GU::Point(button_pointX, button_pointY), GU::Size(30, 10), button_label, true, screen_ptr, true, White_c, White_c);
    
    buttons[Medications_strength_b]->set_property("Name", button_label);
    buttons[Medications_strength_b]->set_property("RightOf", label_label);
    
    
    
}


bool TLD_device::iequals(const std::string& a, const std::string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (std::tolower(a[i]) != std::tolower(b[i]))
            return false;
    return true;
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

    if(current_pointed_to_object_name.str().find("_value")) {
        Trace_out << " Can find a value in current pointed to object name  " << endl;
        
        std::string s = current_pointed_to_object_name.str();
        std::regex reg_exp ("_value$");
        
        current_pointed_to_object_name = std::regex_replace(s, reg_exp, "");
        Trace_out << "This is the value of s now :::: " << s << " " << current_pointed_to_object_name << endl;
        
    }
    
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
    
    fields_t::iterator it_fields = fields.find(current_pointed_to_object_name);
    
    buttons_t::iterator it_menuItems = menuItems.find(current_pointed_to_object_name);
    
    labeledFields_t::iterator it_labeledFields = labeledFields.find(current_pointed_to_object_name);

    
    if(it_buttons == buttons.end() && it_fields == fields.end() && it_menuItems == menuItems.end() && it_labeledFields == labeledFields.end())
        throw Device_exception(this, "Click-on unrecognized object");
    
    
    if(it_menuItems != menuItems.end()){
        Smart_Pointer<Button_widget> current_button_ptr = it_menuItems->second;
        
        Symbol label = "";
        
        bool flag_minInterval = false;
        bool flag_maxOf = false;
        bool flag_forOption = false; 
        bool flag_routeOption = false;
        bool flag_schOption = false;
        
        
        if ( current_pointed_to_object_name == Medications_schSch_options1_mI) {
            deleteMenuItems();
            flag_schOption = true;
            label = "Twice Daily";
        } if ( current_pointed_to_object_name == Medications_schSch_options2_mI) {
            deleteMenuItems();
            flag_schOption = true;
            label = "Three Times Daily";
        } if ( current_pointed_to_object_name == Medications_schSch_options3_mI) {
            deleteMenuItems();
            flag_schOption = true;
            label = "Four Times Daily";
        }
        
        
        if ( current_pointed_to_object_name == Medications_schRoute_options1_mI) {
            deleteMenuItems();
            flag_routeOption = true;
            label = "Left Eye";
        }
        
        
        if(current_pointed_to_object_name == Medications_schFor_options1_mI){
            deleteMenuItems();
            flag_forOption = true;
            label = "day(s)";
        }

        
        
        if(current_pointed_to_object_name == Medications_minInterval_options_1_mI) {
            deleteMenuItems();
            label = "minute(s)";
            flag_minInterval = true;
            
        }
        if(current_pointed_to_object_name == Medications_minInterval_options_2_mI) {
            deleteMenuItems();
            label = "hour(s)";
            flag_minInterval = true;
        }
        if(current_pointed_to_object_name == Medications_minInterval_options_3_mI) {
            deleteMenuItems();
            label = "day(s)";
            flag_minInterval = true;
        }
        
        if(current_pointed_to_object_name == Medications_maxOf_options_1_mI) {
            deleteMenuItems();
            label = "doses";
            flag_maxOf = true; 
        }
        
        if(current_pointed_to_object_name == Medications_maxOf_options_2_mI) {
            deleteMenuItems();
            label = "Tablet";
            flag_maxOf = true; 
        }
        
        if(flag_minInterval == true) {
            
            create_button(this, Medications_third_minInterval_options_b, GU::Point(327, 500), GU::Size(50,10), label, true, screen_ptr, true, White_c, White_c);
            buttons[Medications_third_minInterval_options_b]->set_property(Shape_c, Rectangle_c);
            
        }
        else if (flag_maxOf == true) {
            
            create_button(this, Medications_third_maxOf_options_b, GU::Point(327, 530), GU::Size(50,10), label, true, screen_ptr, true, White_c, White_c);
        }
        else if (flag_forOption == true) {
            create_button(this, Medications_sch_forOptions_b, GU::Point(327, 530), GU::Size(50, 10), label, true, screen_ptr, true, Gray_c, Gray_c);
        }
        else if (flag_routeOption == true) {
            create_button(this, Medications_sch_routeOptions_b, GU::Point(327, 300), GU::Size(50, 10), label, true, screen_ptr, true, Gray_c, Gray_c);
        }
        else if (flag_schOption == true) {
            create_button(this, Medications_sch_schOptions_b, GU::Point(327, 400), GU::Size(50, 10), label, true, screen_ptr, true, Gray_c, Gray_c);
        } 
        
    }
    
    

    if(it_buttons != buttons.end()){
        Smart_Pointer<Button_widget> current_button_ptr = it_buttons->second;
        
        if(!(current_button_ptr->get_state()))
            throw Device_exception(this, "Click-on button that is off");
        
        current_button_ptr->set_state(false);
        
        
        if (current_pointed_to_object_name == Medications_sch_forOptions_b){
            createMenuItems3();
            
            buttons[current_pointed_to_object_name]->depresent();
            screen_ptr->remove_widget(buttons[current_pointed_to_object_name]);
            buttons.erase(current_pointed_to_object_name);
            
        }
        if (current_pointed_to_object_name == Medications_sch_routeOptions_b) {
            createMenuItems4();
            
            buttons[current_pointed_to_object_name]->depresent();
            screen_ptr->remove_widget(buttons[current_pointed_to_object_name]);
            buttons.erase(current_pointed_to_object_name);
        }
        
        if (current_pointed_to_object_name == Medications_sch_schOptions_b) {
            createMenuItems5();
            
            buttons[current_pointed_to_object_name]->depresent();
            screen_ptr->remove_widget(buttons[current_pointed_to_object_name]);
            buttons.erase(current_pointed_to_object_name);
        }
        

        if(current_pointed_to_object_name == ButtonAllergy_b){
            
            clear_objects_on_screen();
            
            
            create_allergies_display(true, false, false);
        } 
        if( current_pointed_to_object_name == Allergy_option3_b) {
            Main_screen_string1 << "Class allergy to";
            create_allergies_display(false, true, false);
        } 

        if( current_pointed_to_object_name == Allergy_option4_b) {
            Main_screen_string1 << "Drug allergy to";
            create_allergies_display(false, true, false);
        }
        
        
        if(current_pointed_to_object_name == Allergy_option6_b){
            
            Main_screen_string1 << "Non-drug allergy to"; 
            
            create_allergies_display(false, false, true);
            
        }
        
        if(current_pointed_to_object_name == Allergy_continue_b){
            clear_objects_on_screen();
            create_homeScreen_display(true);
            
        }
        
        if(current_pointed_to_object_name == "Allergy_search"){
            create_allergies_display(false, true, true);
            
        }
        if ( current_pointed_to_object_name == Medications_cease_scheduled_b) {
        
            create_cease_med(current_searched_medication.str());
        }
        
        
        if(current_pointed_to_object_name == ButtonMedications_d){
            clear_objects_on_screen();
            create_medications_display(true, false, false, false, false, false, false);
        }
        
        if(current_pointed_to_object_name == Medications_prescribe_b){
            clear_objects_on_screen();
            create_medications_display(false, true, false, false, false, false, false);
            
        }
        
        if(current_pointed_to_object_name == Medications_searchButton_b) {
            
            create_medications_display(false, false, true, false, false, false, false);
            
        
        }
        
        if(current_pointed_to_object_name == Medications_searchResults_b){
            Trace_out << " Time to create scheduled medications entery form !!! " << endl;
            
            create_medications_display(false, false, false, true, false, false, false);
            
            
            
        }
        
        
        if( current_pointed_to_object_name == Medications_strength_b){
           
            if( current_searched_medication == PRN_oxycodone){
                create_warning_display();
            }
            else {
                create_medications_display(false, false, false, false, true, false, false);
            }
            
        }
        
        if( current_pointed_to_object_name == Medications_warning_continue_b){
            create_medications_display(false, false, false, false, true, false, false);
        }
        
        
        if( current_pointed_to_object_name == Medications_third_PRN_b ) {
            // TODO::Change part of the display
            // Change medication from scheduled to PRN
            
            buttons_t::iterator it2_buttons = buttons.find(Medications_third_scheduled_b);
            if(it2_buttons != buttons.end()){
                (it2_buttons->second)->set_state(false);
            }
            
            // scheduledMedications->delete;
            
            prnMedications[current_searched_medication] = scheduledMedications[current_searched_medication];
            scheduledMedications.erase(current_searched_medication);
            
            
            create_medications_display(false, false, false, false, false, true, false);
        }
        
        if( current_pointed_to_object_name == Medications_continue_b) {
            clear_objects_on_screen();
            create_medications_display(true, false, false, false, false, false, true);
        }
        
        
        if( current_pointed_to_object_name == Medications_update_b ){
            //clear_objects_on_screen();
            //create_medications_display(true, false, false, false, false, false, true);
            // create_homeScreen_display(true);
            //create_medications_homeDisplay();
            //clear_objects_on_screen(true);
            //create_medications_homeDisplay();
            
        }
        
        if( current_pointed_to_object_name == Medications_close_b ) {
            clear_objects_on_screen();
            create_homeScreen_display(true);
            
        }
        
        if( current_pointed_to_object_name == Medications_third_minInterval_options_b) {
                // create new window, with buttons, where the user can click.
                // Clicking on one of those will remove the window and those buttons but which change the values in the original window.
            
            
            createMenuItems();
            
            buttons[current_pointed_to_object_name]->depresent();
            screen_ptr->remove_widget(buttons[current_pointed_to_object_name]);
            buttons.erase(current_pointed_to_object_name);
            
            
            
        }
        

        if( current_pointed_to_object_name == Medications_third_maxOf_options_b) {
            // Menu options for maximum of, used by oxycodone.
            
            
            createMenuItems2();
            
            buttons[current_pointed_to_object_name]->depresent();
            screen_ptr->remove_widget(buttons[current_pointed_to_object_name]);
            buttons.erase(current_pointed_to_object_name);
            
            
            
        }
        
    }
    else if(it_fields != fields.end()){
        if(current_pointed_to_object_name == Field_AllergySubstance_d){
            // Trace_out << " blah blah blah blah blah blah blah blah blah blah blabh blah blah " << endl;
            
        }
        
    }
	
    
	output_display();
}

void TLD_device::create_cease_med(string medication){
 //   int label_pointX, label_pointY;
    int button_pointX, button_pointY;
    
    Symbol label_label;
    Symbol button_label;
    
    
    // PRN medication chart 1
    if(iequals(medication, "Prednisolone")){
        
  //      label_pointX = 270; label_pointY = 520;
        button_pointX = 420; button_pointY = 520;
        
  //      label_label = "Tablet";
        button_label = Replace_C_amiodarone;
        
    }
    else if ( iequals(medication, "Heparin Sodium")){
        button_pointX = 420; button_pointY = 520;
        
        //      label_label = "Tablet";
        button_label = Replace_C_amiodarone;
    }
    else if ( iequals(medication, "Sotalol")) {
        button_pointX = 420; button_pointY = 520;
        
        //      label_label = "Tablet";
        button_label = Replace_C_amiodarone;
    }
    
    
//    create_label(this, Medications_form_b, GU::Point(label_pointX, label_pointY) , GU::Size(30, 10), label_label, Black_c, true);
    create_button(this, Medications_toCeaseMed_b, GU::Point(button_pointX, button_pointY), GU::Size(30, 10), button_label, true, screen_ptr, true, White_c, White_c);
    
    buttons[Medications_strength_b]->set_property("Name", button_label);
  //  buttons[Medications_strength_b]->set_property("RightOf", label_label);
    
    
    
    
    
}

void TLD_device::deleteMenuItems(){
    
    for(buttons_t::const_iterator it = menuItems.begin(); it != menuItems.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second));
    }
    
    
    menuItems.clear();
    
    for(windows_t::const_iterator it = windows.begin(); it != windows.end(); ++it) {
        (it->second)->depresent();
        screen_ptr->remove_widget((it->second));
    }
    
    windows.clear();
    
    
}

void TLD_device::create_warning_display(){
    clear_objects_on_screen();
    create_button(this, Medications_warning_override_b, GU::Point(290,500), GU::Size(10, 10), "  Override", true, screen_ptr, true, Gray_c, Black_c);
    buttons[Medications_warning_override_b]->set_property("Name", "Override");
    
    // Trace_out << "Over riding :) " << endl;
    
    
    std::vector<GU::Point> in_vertices2 {GU::Point(262, 190), GU::Point( 262, 550)}; // vertical line
    create_polygon(this, Symbol("Polygon1"), in_vertices2, LightBlue_c, true);
    
    create_button(this, Medications_warning_continue_b, GU::Point(600,600), GU::Size(70, 10), "Continue", true, screen_ptr, true, DarkGray_c, DarkGray_c);
    buttons[Medications_warning_continue_b]->set_property("Name", "Continue");
    buttons[Medications_warning_continue_b]->set_property(Shape_c, Rectangle_c);
    
  /*  create_Field(<#Device_base *dev_ptr#>, <#const Symbol &widget_name#>, <#GU::Point location#>, <#GU::Size size#>, <#Symbol new_string#>, <#Symbol color#>, <#bool should_present#>);
    create_label(<#Device_base *dev_ptr#>, <#const Symbol &widget_name#>, <#GU::Point location#>, <#GU::Size size#>, <#const Symbol &label#>, <#const Symbol &color#>, <#bool should_present#>);
    */
    
    create_Object(this, Medications_warning_heading_o, GU::Point(268, 200), GU::Size(300, 20), RoyalBlue_c, Filled_Rectangle_c, "Pregnancy Warnings", true);
    
    
    //create_label(this, Medications_warning_pregnancy_l, GU::Point(318, 550), GU::Size(50, 15), "Pregnancy Warnings", Black_c, true);
    
}



void TLD_device::createMenuItems(){
    
    Smart_Pointer<Window_widget> ptr = new Window_widget(this, "Menu", GU::Point(327, 500), GU::Size(74, 60), "  ");
    
    
     windows["Menu"] = ptr;
    
    // menuItems["Test_container"] = ptr;
    Smart_Pointer<Button_widget> button_ptr = new Button_widget(this, Medications_minInterval_options_1_mI, GU::Point(327, 520), GU::Size(50, 10), "minute(s)", White_c, LightBlue_c, true);
    menuItems[Medications_minInterval_options_1_mI] = button_ptr;
    //menuItems[Medications_minInterval_options_1_mI]->set_property(Shape_c, Rectangle_c);
    
    
    
    Smart_Pointer<Button_widget> button_ptr2 = new Button_widget(this, Medications_minInterval_options_2_mI, GU::Point(327, 535), GU::Size(50, 10), "hour(s)", White_c, White_c, true);
    
    menuItems[Medications_minInterval_options_2_mI] = button_ptr2;
    
    //menuItems[Medications_minInterval_options_2_mI]->set_property(Shape_c, Rectangle_c);
    
    
    Smart_Pointer<Button_widget> button_ptr3 = new Button_widget(this, Medications_minInterval_options_3_mI, GU::Point(327, 550), GU::Size(50, 10), "day(s)", White_c, White_c, true);
    menuItems[Medications_minInterval_options_3_mI] = button_ptr3;
    //menuItems[Medications_minInterval_options_3_mI]->set_property(Shape_c, Rectangle_c);
    
    
    //   buttons["Medications_minInterval_options_3"]->set_property("Name", "MedDay");
    
    
    button_ptr->present_object();
    button_ptr2->present_object();
    button_ptr3->present_object();
    ptr->present_object();
    
    //buttons["Test_button"] = button_ptr;
    screen_ptr->add_widget(button_ptr);
    screen_ptr->add_widget(button_ptr2);
    screen_ptr->add_widget(button_ptr3);
    
    
    button_ptr->set_property("Name", "minute(s)");
    button_ptr2->set_property("Name", "hour(s)");
    button_ptr3->set_property("Name", "day(s)");
    
    button_ptr->set_property(Shape_c, Rectangle_c);
    button_ptr2->set_property(Shape_c, Rectangle_c);
    button_ptr3->set_property(Shape_c, Rectangle_c);
    
    //   ptr->add_widget(button_ptr);
    screen_ptr->add_widget(ptr);

}


void TLD_device::createMenuItems2(){
    
    Smart_Pointer<Window_widget> ptr = new Window_widget(this, "Menu", GU::Point(327, 530), GU::Size(65, 60), "  ");
    
    
    windows["Menu"] = ptr;
    
    // menuItems["Test_container"] = ptr;
    Smart_Pointer<Button_widget> button_ptr = new Button_widget(this, Medications_maxOf_options_1_mI, GU::Point(327, 540), GU::Size(50, 15), "doses", White_c, White_c, true);
    
    menuItems[Medications_maxOf_options_1_mI] = button_ptr;
    
    Smart_Pointer<Button_widget> button_ptr2 = new Button_widget(this, Medications_maxOf_options_2_mI, GU::Point(327, 560), GU::Size(50, 15), "Tablet", White_c, White_c, true);
    
    menuItems[Medications_maxOf_options_2_mI] = button_ptr2;
    
    
    button_ptr->present_object();
    button_ptr2->present_object();

    ptr->present_object();
    
    //buttons["Test_button"] = button_ptr;
    screen_ptr->add_widget(button_ptr);
    screen_ptr->add_widget(button_ptr2);

    
    
    button_ptr->set_property("Name", "doses");
    button_ptr2->set_property("Name", "Tablet");
    
    
    //   ptr->add_widget(button_ptr);
    screen_ptr->add_widget(ptr);
    
}


void TLD_device::createMenuItems3(){
    
    Smart_Pointer<Window_widget> ptr = new Window_widget(this, "Menu", GU::Point(327, 530), GU::Size(65, 60), "  ");
    
    
    windows["Menu"] = ptr;  
    
    // menuItems["Test_container"] = ptr;
    Smart_Pointer<Button_widget> button_ptr = new Button_widget(this, Medications_schFor_options1_mI, GU::Point(327, 540), GU::Size(50, 15), "day(s)", White_c, White_c, true);
    
    menuItems[Medications_schFor_options1_mI] = button_ptr;
        
    button_ptr->present_object();
    
    ptr->present_object();
    
    //buttons["Test_button"] = button_ptr;
    screen_ptr->add_widget(button_ptr);
    
    
    
    button_ptr->set_property("Name", "day(s)");
    
    //   ptr->add_widget(button_ptr);
    screen_ptr->add_widget(ptr);
    
}



void TLD_device::createMenuItems4(){
    
    Smart_Pointer<Window_widget> ptr = new Window_widget(this, "Menu", GU::Point(327, 300), GU::Size(65, 60), "  ");
    
    
    windows["Menu"] = ptr;
    
    // menuItems["Test_container"] = ptr;
    Smart_Pointer<Button_widget> button_ptr = new Button_widget(this, Medications_schRoute_options1_mI, GU::Point(327, 300), GU::Size(50, 15), "Left Eye", White_c, White_c, true);
    
    menuItems[Medications_schRoute_options1_mI] = button_ptr;
    
    button_ptr->present_object();
    
    ptr->present_object();
    
    //buttons["Test_button"] = button_ptr;
    screen_ptr->add_widget(button_ptr);
    
    
    
    button_ptr->set_property("Name", "Left Eye");
    
    //   ptr->add_widget(button_ptr);
    screen_ptr->add_widget(ptr);
    
}



void TLD_device::createMenuItems5(){
    
    Smart_Pointer<Window_widget> ptr = new Window_widget(this, "Menu", GU::Point(327, 400), GU::Size(65, 60), "  ");
    
    
    windows["Menu"] = ptr;
    
    // menuItems["Test_container"] = ptr;
    Smart_Pointer<Button_widget> button_ptr = new Button_widget(this, Medications_schSch_options1_mI, GU::Point(327, 400), GU::Size(50, 15), "Twice Daily", White_c, White_c, true);
    
    menuItems[Medications_schSch_options1_mI] = button_ptr;
    
    
    Smart_Pointer<Button_widget> button_ptr2 = new Button_widget(this, Medications_schSch_options2_mI, GU::Point(327, 420), GU::Size(50, 15), "Three Times Daily", White_c, White_c, true);
    
    menuItems[Medications_schSch_options2_mI] = button_ptr2;
    
    
    Smart_Pointer<Button_widget> button_ptr3 = new Button_widget(this, Medications_schSch_options3_mI, GU::Point(327, 430), GU::Size(50, 15), "Four Times Daily", White_c, White_c, true);
    
    menuItems[Medications_schSch_options3_mI] = button_ptr3;
    
    
    
    button_ptr->present_object();
    button_ptr2->present_object();
    button_ptr3->present_object();
    
    ptr->present_object();
    
    //buttons["Test_button"] = button_ptr;
    screen_ptr->add_widget(button_ptr);
    screen_ptr->add_widget(button_ptr2);
    screen_ptr->add_widget(button_ptr3);
    
    
    
    button_ptr->set_property("Name", "Twice Daily");
    button_ptr2->set_property("Name", "Three Times Daily");
    button_ptr3->set_property("Name", "Four Times Daily");
    
    
    //   ptr->add_widget(button_ptr);
    screen_ptr->add_widget(ptr);
    
}

void TLD_device::handle_Keystroke_event(const Symbol& key_name){

    if(key_name == "DELETE") {
        Trace_out << "------ ===== ===== ======  delete the selected text - " << endl;
    }
    
}





void TLD_device::handle_Type_In_event(const Symbol& type_in_string){
    
    // can set current clicked on object name, and make sure it and current pointed to object name are the same
    fields_t::iterator it_fields = fields.find(current_pointed_to_object_name);
    
    labeledFields_t::iterator it_labeledFields = labeledFields.find(current_pointed_to_object_name);
    
    if(it_fields != fields.end()){
        (it_fields->second)->set_string(type_in_string);
    }
    if(it_labeledFields != labeledFields.end()){
        (it_labeledFields->second)->set_string(type_in_string);
    }
    
    
    if(current_pointed_to_object_name == Field_AllergySubstance_d) {
        Main_screen_string1 << " " << type_in_string;
        
        it_fields = fields.find(Field_AllergyComments_d);
        (it_fields->second)->set_string(" ");
        
        
        // change color or next fields here;  maybe update it in the screen_ptr
    }
    
    if ( current_pointed_to_object_name == Field_AllergyComments_d){
        Main_screen_string1 << " " << type_in_string;

    }
    
    if( current_pointed_to_object_name == Medications_searchDrug_lF){
        current_searched_medication = type_in_string;
        // Trace_out << " ... test ... " << " ...... "  << " ... test ... " << endl;
    }
    
    if( current_pointed_to_object_name == "Medications_third_maxOfValue") {
        
        //fields["Medications_third_maxOfTime"]->set_string(" ");
        
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

