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
  
-- ------------------------------------------------------------
-- Function to insert a new record with dummy eftr_code
-- ------------------------------------------------------------

create or replace function eftr_tr_before()
returns trigger
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_eftr__sequence eftr.eftr__sequence%TYPE;
    min_paym integer;
    max_paym integer;

    BEGIN

         select max(paym__sequence)
         into   max_paym
         from   paym;

         select max(eftr_last__sequence) + 1
         into   min_paym
         from   eftr;

         if ( max_paym is null ) then
            max_paym := 0;
         end if;

         if ( min_paym is null ) then
            min_paym := 0;
         end if;

         if ( max_paym <= min_paym ) then
           RAISE EXCEPTION ''There are no unreported payments'';
           return null;
         end if;

         RAISE NOTICE ''eftr__sequence = %'',new.eftr__sequence;
         new.eftr_first__sequence = min_paym;
         new.eftr_last__sequence = max_paym;
         

    return new;
    END;'
  LANGUAGE 'plpgsql';

drop trigger eftr_tr_before on eftr;
create trigger eftr_tr_before before insert
    on eftr for each row
    execute procedure eftr_tr_before();

-- update history;
\set mttb_name '\'eftr\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
