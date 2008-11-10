--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
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

-- pc-full_20030807.csv
-- "Pcde","Locality","State","Comments","DeliveryOffice","PresortIndicator","ParcelZone","BSPnumber","BSPname","Category"

create table pcde                       -- Postcodes
(
  pcde_postcode text,
  pcde_locality text,
  pcde_state text,
  pcde_comments text,
  pcde_delivery_office text,
  pcde_presort_indicator text,
  pcde_parcelzone text,
  pcde_bsp_number text,
  pcde_bsp_name text,
  pcde_category text,
  --
  -- standard headers
  --
  pcde__sequence serial not null,                               -- Row-ID
  pcde__timestamp timestamp default now() not null,        -- Timestamp
  pcde__user_entry text default CURRENT_USER not null,          -- Entered by
  pcde__status char default text 'N' not null,                  -- Record status
  --
  -- constraints
  --
  primary key(pcde__sequence)
 );

grant select,insert,update on pcde to public;
grant select,insert,update on pcde_pcde__sequence_seq to public;

delete from mtcl where mtcl_name = 'pcde';
select create_metadata('pcde');
update mtcl set mtcl_title = 'Postcodes' where mtcl_name = 'pcde';

update mtat set mtat_displen = 6  where mtat_name = 'pcde_postcode';
update mtat set mtat_displen = 20 where mtat_name = 'pcde_locality';
update mtat set mtat_displen = 5  where mtat_name = 'pcde_state';
update mtat set mtat_displen = 20 where mtat_name = 'pcde_comments';
update mtat set mtat_displen = 20 where mtat_name = 'pcde_delivery_office';
update mtat set mtat_displen = 5  where mtat_name = 'pcde_presort_indicator';
update mtat set mtat_displen = 5  where mtat_name = 'pcde_parcelzone';
update mtat set mtat_displen = 5  where mtat_name = 'pcde_bsp_number';
update mtat set mtat_displen = 20 where mtat_name = 'pcde_bsp_name';
update mtat set mtat_displen = 20 where mtat_name = 'pcde_category';

update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~'^pcde__.*';

-- load from distributed file
select 
  pcde_postcode,
  pcde_locality,
  pcde_state,
  pcde_comments,
  pcde_delivery_office,
  pcde_presort_indicator,
  pcde_parcelzone,
  pcde_bsp_number,
  pcde_bsp_name,
  pcde_category
into temp temp_pcde
from pcde
where 1=0;

copy temp_pcde from '/tmp/pcode.csv' using DELIMITERS ',';

insert into pcde(
  pcde_postcode,
  pcde_locality,
  pcde_state,
  pcde_comments,
  pcde_delivery_office,
  pcde_presort_indicator,
  pcde_parcelzone,
  pcde_bsp_number,
  pcde_bsp_name,
  pcde_category)
select * from temp_pcde where pcde_postcode !~* 'pcde';

update pcde set
  pcde_postcode = trim(both '" ' from pcde_postcode),
  pcde_locality = trim(both '" ' from pcde_locality),
  pcde_state = trim(both '" ' from pcde_state),
  pcde_comments = trim(both '" ' from pcde_comments),
  pcde_delivery_office = trim(both '" ' from pcde_delivery_office),
  pcde_presort_indicator = trim(both '" ' from pcde_presort_indicator),
  pcde_parcelzone = trim(both '" ' from pcde_parcelzone),
  pcde_bsp_number = trim(both '" ' from pcde_bsp_number),
  pcde_bsp_name = trim(both '" ' from pcde_bsp_name),
  pcde_category = trim(both '" ' from pcde_category);

create index ix_pcde_postcode on pcde(pcde_postcode);
create index ix_pcde_locality on pcde(pcde_locality);
create unique index ix_postcode__sequence on pcde(pcde_postcode, pcde__sequence);
create unique index ix_locality__sequence on pcde(pcde_locality, pcde__sequence);

-- update history;
\set mttb_name '\'pcde\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
