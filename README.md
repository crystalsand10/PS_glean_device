Model of the deivce and user of a GP system in order to study its usablity.


* PatientSafety_v1.gomsl - basic model for initial device testing. 
* PatientSafety_v2.gomsl - model for testing of Non - drug allergy option
* PatientSafety_v3.gomsl - model for testing of Class allergy. (Can be substituted with Drug allergy for its testing)
* PatientSafety_v4.gomsl - model for testing of multiple Medications (models prescribe PRN medication for chart 1 perfectly)
* PatientSafety_v5.gomsl - model for bringing previous sub-models together. Aims to model scenario1 of the patient charts. 
* PatientSafety_v6.gomsl - model for testing various object shapes.
* PatientSafety_v7.gomsl - model for testing of medications, but where fields are labeled. 
* PatientSafety_v8.gomsl - leading towards a proper user model for PRN medications, which can be compared against the empirical results. 
* PatientSafety_v9.gomsl - Trying to simplify the actions of the modelled user in version 8. 
* PatientSafety_v10.gomsl - Models the entry of the PRN task 3 accurately, the previous models for PRN task 3 will not work with the currently updated device (due to addition of warning screen and qualifier fields). 
	** This file has been further split into 3 files, in order to get the run times of each of the PRN tasks individually. Each of the file contains just one medication. Apart from that, the implemented methods are the same. 

* PatientSafety_v11.gomsl - Model to test the features (specifically menu Items) of scheduled medications 
* PatientSafety_v12.gomsl - Model to test the features (specifically menu Items) of replace medications 

* PatientSafety_v13.gomsl - leading towards proper methods to carry out replace and scheduled medication prescribing tasks. 

* PatientSafety_v14.gomsl - Complete user model, incorportates all prescribing tasks (not including updating/adding allergy tasks), and methods to carry out prescribing and replacing.

* PatientSafety_v15.gomsl - Model similar to PS_v14, but with additions to the replace medication method, and tasks particular to its testing. 

* PatientSafety_v16.gomsl - Model to test the task of updating a patients allergy status. 

