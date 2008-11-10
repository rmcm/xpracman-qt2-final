--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 

alter table patn add column patn_claim_number text;
alter table patn add column patn_accident_date timestamp;

alter table invc add column invc_hlfd_code text;
alter table invc add column invc_ins_level text;
alter table invc add column invc_healthnumb text;
alter table invc add column invc_healthcard text;
alter table invc add column invc_medicare text;
alter table invc add column invc_claim_number text;
alter table invc add column invc_accident_date timestamp;
alter table invc add column invc_reference_1 text;
alter table invc add column invc_reference_2 text;
alter table invc add column invc_reference_3 text;

select create_metadata('invc');
select create_metadata('patn');

update mtat set mtat_input_method = 10, mtat_title = 'Health Fund', 
                mtat_displen = 10, mtat_view_order = 31
                where mtat_name = 'invc_hlfd_code';
update mtat set mtat_input_method = 10, mtat_title = 'Ins Level', 
                mtat_displen = 10, mtat_view_order = 32 
                where mtat_name = 'invc_ins_level';
update mtat set mtat_input_method = 10, mtat_title = 'Insurance No', 
                mtat_displen = 10, mtat_view_order = 33 
                where mtat_name = 'invc_healthnumb';
update mtat set mtat_input_method = 10, mtat_title = 'Healthcard', 
                mtat_displen = 10, mtat_view_order = 34 
                where mtat_name = 'invc_healthcard';
update mtat set mtat_input_method = 10, mtat_title = 'Medicare No', 
                mtat_displen = 10, mtat_view_order = 34 
                where mtat_name = 'invc_medicare';
update mtat set mtat_input_method = 10, mtat_title = 'Claim No', 
                mtat_displen = 10, mtat_view_order = 35
                where mtat_name = 'invc_claim_number';
update mtat set mtat_input_method = 10, mtat_title = 'Accident Date', 
                mtat_displen = 10, mtat_view_order = 36
                where mtat_name = 'invc_accident_date';
update mtat set mtat_input_method = 10, mtat_title = 'Ref 1', 
                mtat_displen = 10, mtat_view_order = 37
                where mtat_name = 'invc_reference_1';
update mtat set mtat_input_method = 10, mtat_title = 'Ref 2', 
                mtat_displen = 10, mtat_view_order = 38
                where mtat_name = 'invc_reference_2';
update mtat set mtat_input_method = 10, mtat_title = 'Ref 3', 
                mtat_displen = 10, mtat_view_order = 39
                where mtat_name = 'invc_reference_3';

begin work;
select disable_triggers();
update patn set patn_hlfd_code = coalesce(patn_hlfd_code, '-');
alter table patn alter column patn_hlfd_code set not null;
update invc set invc_hlfd_code = coalesce(invc_hlfd_code, patn.patn_hlfd_code),
                        invc_ins_level = coalesce(invc_ins_level, patn.patn_ins_level),
                        invc_healthnumb = coalesce(invc_healthnumb, patn.patn_healthnumb),
                        invc_healthcard = coalesce(invc_healthcard, patn.patn_healthcard),
                        invc_medicare = coalesce(invc_medicare, patn.patn_medicare),
                        invc_claim_number = coalesce(invc_claim_number, patn.patn_claim_number),
                        invc_accident_date = coalesce(invc_accident_date, patn.patn_accident_date)
     from patn
     where invc.invc_patn__sequence = patn.patn__sequence;
select enable_triggers();
commit;

-- register the patch
\set mtpt_name '\'extend-invoiceheaders.sql\''
\set mtpt_rcs_header '\'$Id$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;
