--  -*- sql -*-
--
--     $Id$

--   Copyright 2001 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097


--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public Licensepublished by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
 
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
 
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
--  Report problems and direct all questions to:
 
--      Rex McMaster, rmcm@compsoft.com.au
 

 
--  Revision Log
  


create table surg                       -- Surgical summary
(
 surg_patn__sequence integer,                                   -- Patient ID
 surg_symptoms text,                                            -- Presenting Symptoms
 surg_class text,                                               -- Class
 surg_diag_codes text,                                          -- list of diagnosis codes
 surg_diagnosis text,                                           -- diagnosis description
 surg_treatment text,                                           -- treatment
 surg_oprn_codes text,                                          -- list operation codes
 surg_operation text,                                           -- operation
 surg_oprn_date timestamp,                                      -- operation date.
 surg_oprn_place text,                                          -- operation place.
 surg_photo_code text,                                          -- photo code
 --
 -- standard headers
 --
 surg__sequence serial,                                         -- Row-ID
 surg__timestamp timestamp default now(),                  -- Timestamp
 surg__user_entry text default CURRENT_USER,                    -- Entered by
 surg__status char default text 'N',                            -- Record status
 --
 -- constraints
 --
 primary key (surg__sequence)
);

create index ix_surg_patn on surg(surg_patn__sequence);

grant select,insert,update on surg to public;
grant select,insert,update on surg_surg__sequence_seq to public;


select create_metadata('surg');
update mtat set mtat_input_method = 'RO' where mtat_name ~ '^surg_*';
update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~ '^surg__*';
update mtat set mtat_input_method = 'RO', mtat_displen = 10 where mtat_name = 'surg__timestamp';
update mtat set mtat_input_method = '10', mtat_displen = 15 where mtat_name in (
        'surg_symptoms',
        'surg_class',
        'surg_diag_codes',
        'surg_diagnosis',
        'surg_treatment',
        'surg_oprn_codes',
        'surg_operation',
        'surg_oprn_date',
        'surg_oprn_place',
        'surg_photo_code');
update mtat set mtat_title = 'Symptoms', mtat_view_order = 10 where mtat_name = 'surg_symptoms';
update mtat set mtat_title = 'Class', mtat_view_order = 20 where mtat_name = 'surg_class';
update mtat set mtat_title = 'Diagnosis Codes', mtat_view_order = 30 where mtat_name = 'surg_diag_codes';
update mtat set mtat_title = 'Diagnosis', mtat_view_order = 40 where mtat_name = 'surg_diagnosis';
update mtat set mtat_title = 'Treatment', mtat_view_order = 50 where mtat_name = 'surg_treatment';
update mtat set mtat_title = 'Operation Codes', mtat_view_order = 60 where mtat_name = 'surg_oprn_codes';
update mtat set mtat_title = 'Operation', mtat_view_order = 70 where mtat_name = 'surg_operation';
update mtat set mtat_title = 'Op Date', mtat_view_order = 80 where mtat_name = 'surg_oprn_date';
update mtat set mtat_title = 'Op Place', mtat_view_order = 90 where mtat_name = 'surg_oprn_place';
update mtat set mtat_title = 'Photo', mtat_view_order = 100 where mtat_name = 'surg_photo_code';

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('surg','$Id:');
