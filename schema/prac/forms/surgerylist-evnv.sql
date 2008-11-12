delete from "form" where form_code = 'surgerylist-evnv';
insert into "form" ("form_code","form_desc","form_classname","form_page_attribute","form_includes","form_pagesize","form_orientation","form_mg_top","form_mg_bottom","form_mg_left","form_mg_right","form_rh_height","form_rh_frequency","form_rf_height","form_rf_frequency","form_ph_height","form_ph_frequency","form_pf_height","form_pf_frequency","form_dt_height") values ('surgerylist-evnv','Theatre List','evnv_surgical','evnv_prov_code, evnv_ev_date:evnv_starttime',' ',0,0,10,10,40,40,0,0,0,0,100,1,50,1,721);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,1,'Surgery List:',0,10,120,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,1,'Hospital:',0,30,120,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,1,'Surgeon:',0,50,120,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,1,'Anaesthetist:',0,70,120,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','Anaesthetist',0,1,'.',140,70,300,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,1,'evnv_ev_date',140,10,300,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,1,'evnv_locn_desc',140,30,300,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,1,'evnv_prov_name',140,50,300,20,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,2,'Patient/Procedure',0,40,270,20,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',12,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,2,'Notes',270,40,110,20,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',12,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,2,'Item No:',380,40,140,20,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',12,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,2,'Printed:',0,680,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',0,2,'Page:',425,680,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,2,'evnv_patn_desc_comp:5',0,60,270,100,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',12,75,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,2,'evnv_note_1:5',270,60,110,100,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',12,75,0,0,0,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,2,'evnv_note_2:5',380,60,140,100,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',12,75,0,0,0,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',1,2,'_PAGE',480,680,35,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',12,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','New Item',2,2,'',0,670,515,0,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('surgerylist-evnv','Report Date',200,2,'',55,680,100,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,17,0,36,'255,0,0',0);