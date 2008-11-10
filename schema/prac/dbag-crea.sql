--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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

drop view dbag;
create view dbag                -- invoices - for use on debtor statements.
 as select 
 date(now() - (to_char(conf_integer('dbst_statement_age'),'9999') || ' days')::interval)
                                                                   as dbag_date,        -- aging date.
 dbtr_code                                                         as dbag_dbtr_code,
 dbtr_name                                                         as dbag_name,
 dbtr_address                                                      as dbag_address,     -- Street
 dbtr_suburb                                                       as dbag_suburb,      -- Suburb
 dbtr_postcode                                                     as dbag_postcode,    -- Postcode
 dbtr_state                                                        as dbag_state,       -- State
 dbtr_phone                                                        as dbag_phone,       -- Phone
 dbtr_patient_key(dbtr_code)                                       as dbag_patient_key, -- sort key
 short_date(dbtr_last_statement)                                   as dbag_last_statement,
 dbtr_delay_statement                                              as dbag_delay_statement,
 dbtr_total_balance(dbtr_code,conf_integer('dbst_statement_age'))  as dbag_total_balance,
 dbtr_period_balance(dbtr_code,0,conf_integer('dbst_statement_age'))    as dbag_balance_0,
 dbtr_period_balance(dbtr_code,30,conf_integer('dbst_statement_age'))   as dbag_balance_30,
 dbtr_period_balance(dbtr_code,60,conf_integer('dbst_statement_age'))   as dbag_balance_60,
 dbtr_period_balance(dbtr_code,90,conf_integer('dbst_statement_age'))   as dbag_balance_90,
 dbtr_period_balance(dbtr_code,120,conf_integer('dbst_statement_age'))  as dbag_balance_120,
 --
 -- standard headers
 --
 dbtr__sequence                                                    as dbag__sequence,   -- Row-ID
 dbtr__timestamp                                                   as dbag__timestamp,  -- Timestamp
 dbtr__user_entry                                                  as dbag__user_entry, -- Entered by
 dbtr__status                                                      as dbag__status      -- Record status
 --
 from  dbtr
 where dbtr_total_balance(dbtr_code,conf_integer('dbst_statement_age')) <> 0


-- group by invc_dbtr_code,dbtr_name,dbtr_address,dbtr_suburb,dbtr_postcode,dbtr_state,dbtr_phone,
--          dbag_period,dbtr__sequence,dbtr__timestamp,dbtr__user_entry,dbtr__status
-- having sum(invc_balance(invc__sequence)) <> 0
;

grant select on dbag to public;

select create_metadata('dbag');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('dbag','$Id:');
