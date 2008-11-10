-- Patch to set relation labels
-- $Id$
--
--

update mtcl set mtcl_userlabel = '<b>%1 %2 </b> %3 %4: Invoice #%5, svpf_date_service, svpf_serv_code,svpf_amount,svpf_desc,svpf_invc__sequence' where mtcl_name = 'svpf';

update mtcl set mtcl_userlabel = 'Invoice <b>%1</b> : $%2 + $%3 Paid $%4 + $%5 Debtor Code: %6  Patient ID %7,invc__sequence,invc_amount,invc_gst_amount,invc_paid_amount,invc_paid_gst_amount,invc_dbtr_code,invc_patn__sequence' where mtcl_name = 'invc';

update mtcl set mtcl_userlabel = '<b>%1</b>: %2 %3 %4, dbtr_code,dbtr_name,dbtr_address,dbtr_suburb' where mtcl_name = 'dbtr';

update mtcl set mtcl_userlabel = '<b>%1</b>: %2 %3 %4, rfdr_code,rfdr_name,rfdr_street,rfdr_suburb' where mtcl_name = 'rfdr';

update mtcl set mtcl_userlabel = '<b>%1</b>: %2 %3 $%4 %5 %6 %7,paym__sequence,paym_date_entry,paym_tdtp_code,paym_amount,paym_drawer,paym_bank,paym_branch' where mtcl_name = 'paym';

update mtcl set mtcl_userlabel = '<b>%1 %2 (%3)</b><br><i>%4 %5 %6</i>' ||
                                 '<br>Ref: %7(%8)<br>Ph: %9<br> Last Visit: %+1<br> Owing: %+2<br>,' ||
                                 ' patn_fsnam,patn_psnam, patn_dob,patn_address, patn_suburb, patn_postcode,' ||
                                 ' patn_ref_date,patn_ref_period, patn_phone, patn_last_visit, patn_amount_outstanding'
                                where mtcl_name = 'patn';

update mtcl set mtcl_userlabel = '<b>%1 %2 (%3)</b><br><i>%4 %5 %6</i>,' ||
                                 ' evnv_patn_fsnam,evnv_patn_psnam,evnv_patn_dob,' ||
                                 ' evnv_patn_address,evnv_patn_suburb,evnv_patn_postcode'
                                where mtcl_name = 'evnv';


--update mtcl set mtcl_userlabel = '' where mtcl_name = '';

-- register the patch
\set mtpt_name '\'mtcl-userlabels.sql\''
\set mtpt_rcs_header '\'$Id$\''
insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;
