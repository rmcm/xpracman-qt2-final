delete from "form" where form_code = 'dbstatement';
insert into "form" ("form_code","form_desc","form_classname","form_page_attribute","form_includes","form_pagesize","form_orientation","form_mg_top","form_mg_bottom","form_mg_left","form_mg_right","form_rh_height","form_rh_frequency","form_rf_height","form_rf_frequency","form_ph_height","form_ph_frequency","form_pf_height","form_pf_frequency","form_dt_height") values ('dbstatement','Debtor Statement','dbst','dbst_dbtr_code','lhinvc,creditcard',0,0,10,10,40,40,0,0,0,0,150,1,100,1,621);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Statement of Account',300,76,215,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Invoice',20,110,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Date',70,110,58,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Patient',130,110,148,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Reference',280,110,78,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Amount',365,110,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Paid',415,110,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Unpaid',465,110,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Total Amount Outstanding',0,520,450,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Current',400,536,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'30 Days',400,548,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'60 Days',400,560,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'90 Days',400,572,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'120 Days',400,584,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',0,2,'Page:',400,600,78,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_name',56,0,200,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_address',56,12,200,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_suburb',56,24,200,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_state',56,36,200,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_date',300,50,215,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'_ITEM:30',0,130,18,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_invc__sequence:30',20,130,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_date_printed:30',70,130,58,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_patient:30',130,130,148,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_reference:30',280,130,78,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_amount:30',365,130,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_paid:30',415,130,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_balance:30',465,130,48,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_total_balance',450,520,65,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_balance_0',450,536,65,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_balance_30',450,548,65,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_balance_60',450,560,65,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_balance_90',450,572,65,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'dbst_balance_120',450,584,65,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',1,2,'_PAGE',480,600,35,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',2,2,'',0,94,515,0,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',10,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('dbstatement','New Item',2,2,'',0,510,515,0,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);