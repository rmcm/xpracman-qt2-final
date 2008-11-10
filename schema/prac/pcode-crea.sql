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
-- "Pcode","Locality","State","Comments","DeliveryOffice","PresortIndicator","ParcelZone","BSPnumber","BSPname","Category"

create table pcode                       -- Postcodes
(
  pcode_postcode text,
  pcode_locality text,
  pcode_state text,
  pcode_comments text,
  pcode_delivery_office text,
  pcode_presort_indicator text,
  pcode_parcelzone text,
  pcode_bsp_number text,
  pcode_bsp_name text,
  pcode_category text,
  --
  -- standard headers
  --
  pcode__sequence serial not null,                               -- Row-ID
  pcode__timestamp timestamp default now() not null,        -- Timestamp
  pcode__user_entry text default CURRENT_USER not null,          -- Entered by
  pcode__status char default text 'N' not null,                  -- Record status
  --
  -- constraints
  --
  primary key(pcode__sequence)
 );

grant select,insert,update on pcode to public;
grant select,insert,update on pcode_pcode__sequence_seq to public;

delete from mtcl where mtcl_name = 'pcode';
select create_metadata('pcode');
update mtcl set mtcl_title = 'Postcodes' where mtcl_name = 'pcode';

update mtat set mtat_displen = 6  where mtat_name = 'pcode_postcode';
update mtat set mtat_displen = 20 where mtat_name = 'pcode_locality';
update mtat set mtat_displen = 5  where mtat_name = 'pcode_state';
update mtat set mtat_displen = 20 where mtat_name = 'pcode_comments';
update mtat set mtat_displen = 20 where mtat_name = 'pcode_delivery_office';
update mtat set mtat_displen = 5  where mtat_name = 'pcode_presort_indicator';
update mtat set mtat_displen = 5  where mtat_name = 'pcode_parcelzone';
update mtat set mtat_displen = 5  where mtat_name = 'pcode_bsp_number';
update mtat set mtat_displen = 20 where mtat_name = 'pcode_bsp_name';
update mtat set mtat_displen = 20 where mtat_name = 'pcode_category';

update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~'^pcode__.*';

-- load from distributed file
select 
  pcode_postcode,
  pcode_locality,
  pcode_state,
  pcode_comments,
  pcode_delivery_office,
  pcode_presort_indicator,
  pcode_parcelzone,
  pcode_bsp_number,
  pcode_bsp_name,
  pcode_category
into temp temp_pcode
from pcode
where 1=0;
copy temp_pcode from '/tmp/pcode.csv' using DELIMITERS ',';
insert into pcode(
  pcode_postcode,
  pcode_locality,
  pcode_state,
  pcode_comments,
  pcode_delivery_office,
  pcode_presort_indicator,
  pcode_parcelzone,
  pcode_bsp_number,
  pcode_bsp_name,
  pcode_category)
select * from temp_pcode where pcode_postcode !~* 'pcode';

update pcode set
  pcode_postcode = trim(both '" ' from pcode_postcode),
  pcode_locality = trim(both '" ' from pcode_locality),
  pcode_state = trim(both '" ' from pcode_state),
  pcode_comments = trim(both '" ' from pcode_comments),
  pcode_delivery_office = trim(both '" ' from pcode_delivery_office),
  pcode_presort_indicator = trim(both '" ' from pcode_presort_indicator),
  pcode_parcelzone = trim(both '" ' from pcode_parcelzone),
  pcode_bsp_number = trim(both '" ' from pcode_bsp_number),
  pcode_bsp_name = trim(both '" ' from pcode_bsp_name),
  pcode_category = trim(both '" ' from pcode_category);

create index ix_pcode_postcode on pcode(pcode_postcode);
create index ix_pcode_locality on pcode(pcode_locality);

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('pcode','$Id$');
