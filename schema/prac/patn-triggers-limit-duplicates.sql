-- patn-triggers-limit-duplicates.sql ---
-- -*- SQL[ansi] -*-
-- Copyright (C) 2010 Rex McMaster
-- Author: Rex McMaster rex@mcmaster.id.au
-- Created: Fri Jan 22 12:10:11 2010

-- This program is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License as
-- published by the Free Software Foundation; either version 2, or (at
-- your option) any later version.

-- This program is distributed in the hope that it will be useful, but
-- WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- General Public License for more details.

-- You should have received a copy of the GNU General Public License
-- along with this program; see the file COPYING.  If not, write to
-- the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
-- Boston, MA 02110-1301, USA.


-- ------------------------------------------------------------
-- check referror (patn) - don't use retired referrors
-- ------------------------------------------------------------

create or replace function fn_patn_limit_duplicates()
returns trigger
as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    x_count integer;

    BEGIN
        -- RAISE NOTICE 'fn_patn_limit_duplicates: patn__sequence = %', new.patn__sequence;

        -- ----------------------------------------
        -- checks only apply to changes in firstname
        -- from null
        -- ----------------------------------------
	if ( not (old.patn_fsnam is null and
	      new.patn_fsnam is not null) ) then
	   return new;
	end if;

        -- ----------------------------------------
        -- the surname must be valid
        -- ----------------------------------------
	if ( new.patn_psnam is null ) then
	   RAISE EXCEPTION 'The Surname must be valid';
	   return null;
	end if;

        -- ----------------------------------------
        -- check for duplicate surname and firstname
	-- combinations
        -- ----------------------------------------
        select count(*)
        into   x_count
        from   patn
        where upper(patn_psnam) = upper(new.patn_psnam)
        and   upper(patn_fsnam) = upper(new.patn_fsnam)
	and   (new.patn_feet_code is null or (patn_feet_code = new.patn_feet_code))
	and   (new.patn_dob is null or (date(patn_dob) = date(new.patn_dob)))
	and   patn__sequence != new.patn__sequence;

        if ( x_count > 0 ) then
           RAISE EXCEPTION 'The name % % (%, %, %) already exists (%). Try entering the fee-level, DOB and postcode before entering the first name.',
                 new.patn_fsnam, new.patn_psnam, new.patn_feet_code, 
		 coalesce(date(new.patn_dob)::text, ''), coalesce(new.patn_postcode, ''), x_count;
            return null;
        end if;

    return new;

    END;
  $$ LANGUAGE 'plpgsql';

drop trigger tr_patn_limit_duplicates on patn;
create trigger tr_patn_limit_duplicates before update
    on patn for each row
    execute procedure fn_patn_limit_duplicates();


-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'patn-triggers_limit-duplicates\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


-- end patn-triggers-limit-duplicates.sql

