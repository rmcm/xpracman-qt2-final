delete from "form" where form_code = 'invoice-svpv_base';
insert into "form" ("form_code","form_desc","form_classname","form_page_attribute","form_includes","form_pagesize","form_orientation","form_mg_top","form_mg_bottom","form_mg_left","form_mg_right","form_rh_height","form_rh_frequency","form_rf_height","form_rf_frequency","form_ph_height","form_ph_frequency","form_pf_height","form_pf_frequency","form_dt_height") values ('invoice-svpv_base','Basic Invoice layout','svpv','svpv_invc__sequence','lhinvc,creditcard',0,0,10,10,40,40,0,0,0,0,150,1,110,1,611);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,1,'Account to:',48,136,180,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,1,'Patient:',320,136,80,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,1,'svpv_date_printed',415,120,100,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'DOB:',330,48,30,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Tax Invoice',300,76,215,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Service Date',0,110,80,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Item',80,110,50,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Description',130,110,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Fee',350,110,70,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'GST',420,110,80,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,2,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'INVOICE SUMMARY',270,360,230,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Page:',400,360,78,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Total Fees:',270,384,120,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'NOTES',0,390,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'GST:',270,396,120,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'RECEIPTS',270,408,130,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'INVOICE BALANCE',270,480,130,15,'240,240,240','0,0,0','0,0,0',0,0,'Helvetica',10,75,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'TOTAL OUTSTANDING',270,495,150,20,'240,240,240','0,0,0','0,0,0',0,0,'Helvetica',10,75,0,0,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'REMITTANCE',0,512,70,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,75,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Invoice Number:',270,530,120,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Invoice Date:',270,542,120,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Reprinted:',270,554,120,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',0,2,'Invoice Balance:',270,566,120,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_patn_name',330,0,170,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_name',56,12,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_patn_address',330,12,170,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_address',56,24,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_patn_suburb',330,24,170,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_suburb',56,36,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_patn_state',330,36,170,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_state',56,48,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_patn_dob',362,48,170,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_date_service:8',0,130,78,24,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,0,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_serv_code:8',80,130,48,24,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,0,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_desc:8',130,130,240,24,'255,255,255','0,0,0','0,0,0',1,0,'Helvetiva',10,25,0,0,0,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_amount:8',370,130,50,24,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,0,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_gst_amount:8',420,130,80,24,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,0,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'_PAGE',480,360,35,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',8,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_invc_amount',420,384,95,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_invc_gst_amount',420,396,95,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_invc_credits',420,408,95,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_cred_summary',270,420,245,55,'255,255,255','0,0,0','0,0,0',1,0,'Courier',8,25,1,1,0,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_invc_balance',400,480,115,15,'240,240,240','0,0,0','0,0,0',0,0,'Helvetica',10,75,0,2,1,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_patn_dbtr_balance',420,495,95,20,'240,240,240','0,0,0','0,0,0',0,0,'Helvetica',10,75,0,2,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_name',0,522,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_invc__sequence',400,530,115,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_address',0,534,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_date_printed',400,542,115,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_suburb',0,546,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_date_reprint',400,554,115,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_dbtr_state',0,558,220,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,0,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',1,2,'svpv_invc_balance',400,566,115,12,'255,255,255','0,0,0','0,0,0',1,0,'Helvetica',10,25,0,2,2,0,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',2,2,'',0,94,515,0,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',10,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',2,2,'',270,378,245,0,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',10,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',2,2,'',0,408,220,0,'255,255,255','0,0,0','0,0,0',1,1,'Helvetica',10,25,0,1,1,1,0,7,0,36,'255,0,0',0);
insert into "fmdt" ("fmdt_form_code","fmdt_desc","fmdt_field_type","fmdt_section","fmdt_text","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_bg_colour","fmdt_fg_colour","fmdt_bd_colour","fmdt_bd_width","fmdt_bd_style","fmdt_ft_family","fmdt_ft_size","fmdt_ft_weight","fmdt_ft_italic","fmdt_hz_align","fmdt_vt_align","fmdt_word_wrap","fmdt_data_type","fmdt_date_format","fmdt_precision","fmdt_currency","fmdt_neg_colour","fmdt_comma_sep") values ('invoice-svpv_base','New Item',2,2,'',75,518,440,0,'255,255,255','0,0,0','0,0,0',1,2,'Helvetica',10,25,0,1,1,1,0,7,0,36,'255,0,0',0);
