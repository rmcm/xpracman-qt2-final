--     $Id$
-- 
--   Copyright 2004 X=X Computer Software Trust
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

-- Create a virtual attribute for expired referrals, determined
-- from referral date and period.
-- Also consider if a more recent service code 104 has been provided

-- previous function structure
drop function isreferralexpired( timestamp, integer );
create or replace function isreferralexpired( timestamp, integer, integer )
returns char(1) as $$
	DECLARE
        ref_date alias for $1;
        ref_period alias for $2;
        a_patn__sequence alias for $3;
        x_use_ref_date timestamp;

    BEGIN
        
        -- undetermined
        if (ref_date is null or ref_period is null) then
          return 'U';
        end if;

        -- indefinite
        if (ref_period = 99) then
          return 'N';
        end if;

        -- are we within the referral period ?
        if (date(ref_date) + (ref_period||' months')::interval) >= date(now() ) then
          return 'N';
        end if;

        -- find most recent 104 - and compare to that date.
        select  max(svpf_date_service)
        into    x_use_ref_date
        from    svpf
        where   svpf_patn__sequence = a_patn__sequence
        and		svpf_serv_code = '104'
        and 	svpf_date_service > ref_date;
        
        -- no 104's, then expired.        
        if ( x_use_ref_date is null ) then
          return 'Y';
        end if;

        if ( (date(x_use_ref_date) + (ref_period||' months')::interval) >= date(now()) ) then
          return '+';	-- not expired
        else
       	  return 'N';		-- not expired
        end if;
    END;
$$ LANGUAGE 'plpgsql';

-- add patn_ref_expired
delete from mtal where mtal_mtat_name = 'patn_ref_expired';
delete from mtat where mtat_name = 'patn_ref_expired';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, 
				 mtat_browse_order, mtat_view_order, mtat_input_method)
	values('patn_ref_expired', 'char(1)', 'Referral Expired', 'patn', 3, 
				 10, 182, 'VIRTUAL=isreferralexpired( patn_ref_date, patn_ref_period, patn__sequence)');
-- alert
insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority)
        values('patn_ref_expired',3, 'Y','#FF0000', 1);


-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
