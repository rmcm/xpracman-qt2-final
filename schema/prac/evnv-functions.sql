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
 

-- 
-- Create a function to insert via view
-- This function returns the serial_number of the new "virtual" row.
-- This is used instead of an "insert" rule, as the latter cannot use "lastoid"
-- to revover the inserted row.
-- 
drop function evnv_insert_row();
create function evnv_insert_row()
returns integer
as '
    DECLARE
         rec_evnv evnv%ROWTYPE;
         tmp_evnt__sequence evnt.evnt__sequence%TYPE; 
         rec_patn patn%ROWTYPE;
                
    BEGIN

         -- Do not know patient yet, so use 0 
         if ( rec_evnv.evnv_patn__sequence is null ) then
           -- new invoice = 0 to trigger
           rec_evnv.evnv_patn__sequence := 0;
            select  *
            into    rec_patn
            from    patn
            where   patn__sequence = rec_evnv.evnv_patn__sequence;
            -- if not found create a dummy to temporarily satisfy constraints
            if ( not found ) then
              insert into patn(patn__sequence) values(0);
            end if;
         end if;

         -- get a evnt__sequence
         select nextval(''evnt_evnt__sequence_seq'')
         into tmp_evnt__sequence;
         -- add the evnt record 
         insert into evnt(
                     evnt_patn__sequence,
                     evnt__sequence)
               values(
                     rec_evnv.evnv_patn__sequence,
                     tmp_evnt__sequence);

    return tmp_evnt__sequence;
    END;'
    LANGUAGE 'plpgsql';


-- check for referral expiries

create or replace function referral_expires( timestamp, integer, integer )
returns date as '
	DECLARE
        ref_date alias for $1;
        ref_period alias for $2;
        a_patn__sequence alias for $3;
        x_104_ref_date timestamp;

    BEGIN
        
        -- find more recent 104
        select  max(svpf_date_service)
        into    x_104_ref_date
        from    svpf
        where   svpf_patn__sequence = a_patn__sequence
        and		svpf_serv_code = ''104''
        and 	svpf_date_service > ref_date;
        
        if ( ref_period = 0 or ref_period = 99 or ref_period is null or ref_date is null
                        or coalesce(x_104_ref_date,ref_date) is null ) then
        	return null::date;
        end if;
        
        if ( x_104_ref_date is null or x_104_ref_date < ref_date ) then
          return date(ref_date) + (ref_period||'' months'')::interval;
        else
          return date(x_104_ref_date) + (ref_period||'' months'')::interval;
        end if;
    END;
' LANGUAGE 'plpgsql';

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('evnv','$Id$');

