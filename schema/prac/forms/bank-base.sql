delete from form where form_code = 'bank-base';
insert into "form" ("form_classname","form_dt_height","form_mg_bottom","form_mg_left","form_mg_right","form_mg_top","form_orientation","form_pagesize","form_pf_frequency","form_pf_height","form_ph_frequency","form_ph_height","form_rf_frequency","form_rf_height","form_rh_frequency","form_rh_height","form_code","form_desc","form_page_attribute","form_includes") values('','720','10','40','40','10','0','0','1','50','1','80','0','35','0','0','bank-base','Base Bank report','','');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','2','2','36','Amount','bank-base','New Item','Helvetica','0','2','400','70','115','20','0','7','0','0','0','1');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','Printed:','bank-base','Timestamp','Helvetica','0','3','0','5','38','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','36','Page #','bank-base','New Item','Helvetica','0','3','450','5','38','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','Ruler','bank-base','New Item','Helvetica','2','3','0','0','515','0','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','','bank-base','Report Date','Helvetica','200','3','40','5','100','12','0','17','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','36','','bank-base','Page counter','Helvetica','201','3','495','5','20','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','1','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','','bank-base','Ruler','Helvetica','2','2','0','100','515','0','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','36','bkdr_amount:26','bank-base','New Item','Helvetica','1','2','400','120','115','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','Account Name','bank-base','=Account Name','Helvetica','0','1','50','50','100','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','XXX','bank-base','=Account BSB','Helvetica','0','1','50','70','100','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','XXX','bank-base','=Account Number','Helvetica','0','1','50','90','100','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','36','Name:','bank-base','New Item','Helvetica','0','1','0','50','45','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','36','BSB:','bank-base','New Item','Helvetica','0','1','0','70','45','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','2','2','36','ACC#:','bank-base','New Item','Helvetica','0','1','0','90','45','12','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','0','2','36','Branch','bank-base','New Item','Helvetica','0','2','320','70','75','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','0','2','36','Bank','bank-base','New Item','Helvetica','0','2','270','70','45','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','0','2','36','Drawer','bank-base','New Item','Helvetica','0','2','170','70','95','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','bkdr_drawer:26','bank-base','New Item','Helvetica','1','2','170','120','95','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','bkdr_bank:26','bank-base','New Item','Helvetica','1','2','270','120','45','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','8','25','0','2','36','bkdr_branch:26','bank-base','New Item','Helvetica','1','2','320','120','75','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','0','2','36','bkdr_tdtp_desc','bank-base','New Item','Helvetica','1','2','310','40','130','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','2','2','36','bkdr_tdtp_total','bank-base','New Item','Helvetica','1','2','435','40','80','20','0','7','0','0','0','1');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','0','2','36','bkdr_list_name','bank-base','New Item','Helvetica','1','2','310','20','130','20','0','7','0','0','0','0');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','2','2','36','bkdr_list_total','bank-base','New Item','Helvetica','1','2','435','20','80','20','0','7','0','0','0','1');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','2','2','36','bkdr_grand_total','bank-base','New Item','Helvetica','1','2','435','0','80','20','0','7','0','0','0','1');
insert into "fmdt" ("fmdt_bd_width","fmdt_bd_style","fmdt_bd_colour","fmdt_bg_colour","fmdt_fg_colour","fmdt_neg_colour","fmdt_ft_size","fmdt_ft_weight","fmdt_hz_align","fmdt_vt_align","fmdt_currency","fmdt_text","fmdt_form_code","fmdt_desc","fmdt_ft_family","fmdt_field_type","fmdt_section","fmdt_x_coord","fmdt_y_coord","fmdt_width","fmdt_height","fmdt_ft_italic","fmdt_date_format","fmdt_precision","fmdt_data_type","fmdt_comma_sep","fmdt_word_wrap") values('1','0','0,0,0','255,255,255','0,0,0','255,0,0','10','75','0','2','36','Report Total','bank-base','New Item','Helvetica','0','2','310','0','130','20','0','7','0','0','0','0');