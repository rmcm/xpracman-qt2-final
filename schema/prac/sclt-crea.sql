 --  -*- sql -*-
--
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
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
  
create table sclt                       -- Combined service and credit list
(
 sclt_date date,                                                        -- Transaction date
 sclt_patn_name text,                                                   -- Patient Name
 sclt_dc text,                                                          -- Transaction Type D/C
 sclt_prov_code text,                                                   -- Provider Code
 sclt_prov_name text,                                                   -- Provider Name
 sclt_type text,                                                        -- Service code or tender type
 sclt_desc text,                                                        -- Description
 sclt_invc__sequence integer,                                           -- Invoice number
 sclt_debit numeric(12,2),                                              -- Debit Amount
 sclt_gst_debit numeric(12,2),                                          -- Debit GST amount
 sclt_credit numeric(12,2),                                             -- Credit Amount
 sclt_gst_credit numeric(12,2),                                         -- Credit GST amount
 sclt_total_amount numeric(12,2),                                       -- Total Amount D=+, C=-

 --
 -- standard headers
 --
 sclt__sequence serial not null,                                -- Row-ID
 sclt__timestamp timestamp default now() not null,              -- Timestamp
 sclt__user_entry text default CURRENT_USER not null,           -- Entered by
 sclt__status char default text 'N' not null                    -- Record status
 --
 -- constraints
 --

 -- no contraints - this table is just a scratchpad

 );


grant select,insert,update,delete on sclt to public;
grant select,insert,update,delete on sclt_sclt__sequence_seq to public; 

select create_metadata('sclt');

-- update history;
\set mttb_name '\'sclt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
