export interface Review {
  id: number;
  username: string;
  comment: string;
  rating: number;
  date_posted: string;
  likeCount: string[];
  dislikeCount: string[];
  edited: boolean;
  last_edited: string | null;
}
