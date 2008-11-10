--  -*- sql -*-
--  
--    $Id$

--  Copyright 2005 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log
  
create table hpcl                                       -- HIC Patient Claim Log
	(                                               -- : can be >1 per invoice

  hpcl_patn__sequence integer,                          -- Patient file number
  hpcl_prov_code text,                                  -- Service provider code
  hpcl_rfdr_code text,                                  -- Referrer code
  hpcl_locn_code text,                                  -- Location ID
  hpcl_invc__sequence integer,                          -- Invoice number
  hpcl_transaction_id text,                             -- HIC-assigned claim id
  hpcl_dateoflodgement text,                            -- Used for SDD
  hpcl_timeoflodgement text,                            -- Used for SDD
  hpcl_storeandforward text,                            -- Claim may be stored here for later transmission
 --
 -- standard headers
 --
  hpcl__sequence serial,                                -- Row-ID
  hpcl__timestamp timestamp default now(),              -- Timestamp
  hpcl__user_entry text default CURRENT_USER,           -- Entered by
  hpcl__status char default text 'N',                   -- Record status
 --
 -- constraints
 --
 primary key (hpcl__sequence),

 constraint hpcl_fk_patn
            foreign key (hpcl_patn__sequence)
            references patn
            on update cascade
            on delete restrict
            initially deferred,

 constraint hpcl_fk_prov
            foreign key (hpcl_prov_code)
            references prov
            on update cascade
            on delete restrict
            initially deferred,

 constraint hpcl_fk_rfdr
            foreign key (hpcl_rfdr_code)
            references rfdr
            on update cascade
            on delete restrict
            initially deferred,

 constraint hpcl_fk_locn
            foreign key (hpcl_locn_code)
            references locn
            on update cascade
            on delete restrict
            initially deferred,

 constraint hpcl_fk_invc
            foreign key (hpcl_invc__sequence)
            references invc
            on update cascade
            on delete restrict
            initially deferred
);



grant select,insert,update on hpcl to public;
grant select,insert,update on hpcl_hpcl__sequence_seq to public;

select create_metadata('hpcl');
update mtcl set mtcl_title = 'HIC Patient Claim', mtcl_order_by = 'order by hpcl__timestamp desc' where mtcl_name = 'hpcl';
update mtat set mtat_input_method = 'RO' where mtat_class_name = 'hpcl';

update mtat set mtat_title = 'Patient ID', mtat_displen = 15 where mtat_name = 'hpcl_patn__sequence';
update mtat set mtat_title = 'Provider', mtat_displen = 15  where mtat_name = 'hpcl_prov_code';
update mtat set mtat_title = 'Referrer', mtat_displen = 15 where mtat_name = 'hpcl_rfdr_code';
update mtat set mtat_title = 'Location', mtat_displen = 15 where mtat_name = 'hpcl_locn_code';
update mtat set mtat_title = 'Claimed', mtat_displen = 20 where mtat_name = 'hpcl_date_printed';
update mtat set mtat_title = 'Invoice', mtat_displen = 10  where mtat_name = 'hpcl_invc__sequence';
update mtat set mtat_title = 'Claim ID', mtat_displen = 24 where mtat_name = 'hpcl_transaction_id';

-- update history;
\set mttb_name '\'hpcl\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
