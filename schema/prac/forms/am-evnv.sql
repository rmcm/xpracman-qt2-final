delete from form where form_code = 'am-evnv';
insert into "form" ("form_classname","form_dt_height","form_mg_bottom","form_mg_left","form_mg_right","form_mg_top","form_orientation","form_pagesize","form_pf_frequency","form_pf_height","form_ph_frequency","form_ph_height","form_rf_frequency","form_rf_height","form_rh_frequency","form_rh_height","form_code","form_desc","form_page_attribute","form_includes") values('evnv_am','771','10','40','40','10','0','0','1','50','1','50','0','0','0','0','am-evnv','AM Appointments','evnv_prov_code, evnv_ev_date:evnv_starttime',' ');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','14','75','1','1','AM Appointments','am-evnv','New Item','Helvetica','0','1','0','10','515','20','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','1','Provider:','am-evnv','New Item','Helvetica','0','2','0','0','75','20','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','2','1','Date:','am-evnv','New Item','Helvetica','0','2','270','0','80','20','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','2','Start','am-evnv','New Item','Helvetica','0','2','0','30','38','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','2','Patient','am-evnv','New Item','Helvetica','0','2','60','30','118','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','2','Phone','am-evnv','New Item','Helvetica','0','2','180','30','58','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','Printed on:','am-evnv','New Item','Helvetica','0','2','0','730','50','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','Page:','am-evnv','New Item','Helvetica','0','2','425','730','50','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','25','0','1','evnv_prov_name','am-evnv','New Item','Helvetica','1','2','80','0','200','20','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','0','1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','75','0','1','evnv_ev_date','am-evnv','New Item','Helvetica','1','2','355','0','150','20','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','2','T','am-evnv','New Item','Helvetica','0','2','39','30','20','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','75','0','1','evnv_patn_desc:20','am-evnv','New Item','Helvetica','1','2','60','60','118','30','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','1','evnv_patn_phone:20','am-evnv','New Item','Helvetica','1','2','180','60','78','30','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','1','evnv_rfdr_name:20','am-evnv','New Item','Helvetica','1','2','260','60','98','30','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','_PAGE','am-evnv','New Item','Helvetica','1','2','480','730','35','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','1','0,0,0','255,255,255','0,0,0','255,0,0','10','25','1','1','','am-evnv','New Item','Helvetica','2','2','0','25','515','0','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','1','0,0,0','255,255,255','0,0,0','255,0,0','10','25','1','1','','am-evnv','New Item','Helvetica','2','2','0','45','515','0','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','0','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','','am-evnv','New Item','Helvetica','2','2','0','720','515','0','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','0','1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','','am-evnv','Report Date','Helvetica','200','2','55','730','100','12','0','17','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','2','Notes','am-evnv','New Item','Helvetica','0','2','360','30','155','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','1','1','0','0,0,0','255,255,255','0,0,0','255,0,0','12','75','0','2','Referrer','am-evnv','New Item','Helvetica','0','2','260','30','98','12','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','0','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','1','evnv_desc:20','am-evnv','New Item','Helvetica','1','2','360','60','155','30','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','0','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','1','evnv_ev_time:20','am-evnv','New Item','Helvetica','1','2','0','60','38','30','0','7','0');
insert into "fmdt" ("fmdt_data_type","fmdt_currency","fmdt_comma_sep","fmdt_word_wrap","fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision") values('0','36','0','0','1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','1','evnv_apst_code:20','am-evnv','New Item','Helvetica','1','2','39','60','20','30','0','7','0');
