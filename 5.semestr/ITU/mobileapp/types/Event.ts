import { Review } from "./Review";

export interface Event {
  id: number;
  name: string;
  date_of_creation: string;
  available_tickets: number | null;
  date_of_event: string;
  event_category: EventCategory;
  address: string;
  image: string;
  organizer: string;
  region: Region;
  city: string;
  description: string;
  average_rating: number | null;
  reviews: Review[];
  review_count: number | null;
}

export type EventCategory = 'entertainment' | 'education' | 'for_adults' | 'for_children' | 'workshop' | 'exposition';

export type Region = 'praha' | 'stredocesky' | 'jihocesky' | 'plzensky' | 'karlovarsky' | 
                    'ustecky' | 'liberecky' | 'kralovehradecky' | 'pardubicky' | 'vysocina' | 
                    'jihomoravsky' | 'olomoucky' | 'moravskoslezsky' | 'zlinsky' | '';
