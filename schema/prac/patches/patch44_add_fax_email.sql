--     $Id$
-- 
--   Copyright 2006 X=X Computer Software Trust
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


-- add fax and email fields 

alter table patn add column patn_fax_number text;
alter table patn add column patn_email_address text;

alter table dbtr add column dbtr_fax_number text;
alter table dbtr add column dbtr_email_address text;

alter table rfdr add column rfdr_fax_number text;
alter table rfdr add column rfdr_email_address text;

alter table empl add column empl_fax_number text;
alter table empl add column empl_email_address text;

alter table hlfd add column hlfd_fax_number text;
alter table hlfd add column hlfd_email_address text;

alter table prov add column prov_fax_number text;
alter table prov add column prov_email_address text;

alter table locn add column locn_fax_number text;
alter table locn add column locn_email_address text;

select create_metadata('patn');
select create_metadata('dbtr');
select create_metadata('rfdr');
select create_metadata('empl');
select create_metadata('hlfd');
select create_metadata('prov');
select create_metadata('locn');

-- register the patch
\set mtpt_name '\'/patch44_add_fax_email.sql\''
\set mtpt_rcs_header '\'$Id:$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;

