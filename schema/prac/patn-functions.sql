--  -*- sql -*-
--  
--    $Id$

--  Copyright 2000 X=X Computer Software Trust
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
 

-- Functions associated with patn class

-- ------------------------------------------------------------
-- Return formatted patient name
-- ------------------------------------------------------------

create or replace function patn_patient_name(integer)
returns text as $$
  DECLARE
    arg_patn__sequence alias for $1;
    rec_patn patn%ROWTYPE;
    patient_name text;

    BEGIN
    select coalesce(patn_psnam,'')  || ', ' || coalesce(patn_fsnam, '')
    into   patient_name
    from   patn
    where  patn__sequence = arg_patn__sequence;

    if (not found) then
      return '';
    end if;
      
    
    return patient_name;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Return formatted patient name
-- ------------------------------------------------------------

create or replace function patn_patient_title_name(integer)
returns text as $$

  DECLARE
    arg_patn__sequence alias for $1;
    rec_patn patn%ROWTYPE;
    patient_name text;

    BEGIN
    select coalesce(patn_title,'')  ||
           ' ' || 
           coalesce( substr(patn_fsnam,1,1)||'.', '') ||
           coalesce(patn_psnam,'')
    into   patient_name
    from   patn
    where  patn__sequence = arg_patn__sequence;

    if (not found) then
      return '';
    end if;
      
    
    return patient_name;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Return formatted patient address
-- ------------------------------------------------------------

create or replace function patn_patient_address(integer)
returns text as $$

  DECLARE
    arg_patn__sequence alias for $1;
    rec_patn patn%ROWTYPE;
    patient_address text;

    BEGIN
    select coalesce(patn_address,'')  ||
           '<br>' || 
           coalesce(patn_suburb, '') || ' ' || coalesce(patn_postcode, '')
    into   patient_address
    from   patn
    where  patn__sequence = arg_patn__sequence;

    if (not found) then
      return '';
    end if;
      
    
    return patient_address;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Total outstanding for patient
-- ------------------------------------------------------------
create or replace function patn_total_balance(integer,integer)
  returns numeric as $$
  DECLARE
    a_patn__sequence ALIAS for $1;
    a_days_ago ALIAS for $2;
    x_amount numeric := 0.0;
    x_days_ago interval := a_days_ago::text || ' days';

  BEGIN
    select      coalesce(sum(invc_balance_then(invc__sequence,a_days_ago)),0) as total_balance
    into        x_amount
    from        invc
    where       invc_patn__sequence = a_patn__sequence
    and         invc_date_printed is not null
    and         date(invc_date_printed) <= now() - x_days_ago;

  return (((ceil(x_amount * 20) * 5)::numeric)/100)::numeric(12,2);
  END;
$$
LANGUAGE PLPGSQL;

-- ------------------------------------------------------------
-- Period outstanding for patient
-- ------------------------------------------------------------
create or replace function patn_period_balance(integer,integer,integer)
  returns numeric as $$
  DECLARE
    a_patn__sequence ALIAS for $1;
    a_period ALIAS for $2;
    a_days_ago ALIAS for $3;
    x_amount numeric := 0.0;
    x_days_ago interval := a_days_ago::text || ' days';

  BEGIN
    select      coalesce(sum(invc_balance_then(invc__sequence,a_days_ago)),0) as period_balance
    into        x_amount
    from        invc
    where       invc_patn__sequence = a_patn__sequence
    and         invc_date_printed is not null
    and         date(invc_date_printed) <= now() - x_days_ago
    and         invc_age_period(invc__sequence, a_days_ago) = a_period;

  return (((ceil(x_amount * 20) * 5)::numeric)/100)::numeric(12,2);
  END;
$$
LANGUAGE PLPGSQL;

-- ------------------------------------------------------------
-- Total outstanding for patient/debtor
-- ------------------------------------------------------------
create or replace function patn_total_debtor_balance(integer,integer,text)
  returns numeric as $$
  DECLARE
    a_patn__sequence ALIAS for $1;
    a_days_ago ALIAS for $2;
    a_dbtr_code ALIAS for $3;
    x_amount numeric := 0.0;
    x_days_ago interval := a_days_ago::text || ' days';

  BEGIN
    select      coalesce(sum(invc_balance_then(invc__sequence,a_days_ago)),0) as total_balance
    into        x_amount
    from        invc
    where       invc_patn__sequence = a_patn__sequence
    and         invc_dbtr_code = a_dbtr_code
    and         invc_date_printed is not null
    and         date(invc_date_printed) <= now() - x_days_ago;

  return (((ceil(x_amount * 20) * 5)::numeric)/100)::numeric(12,2);
  END;
$$
LANGUAGE PLPGSQL;

