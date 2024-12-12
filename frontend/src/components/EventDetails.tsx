import React, { useState, useEffect } from 'react';
import { useParams, Link, useNavigate } from 'react-router-dom';
import EventImage from './EventImage';
import { useUser } from '../UserContext'; 
import EditReviewModal from './EditReviewModal';
import Rating from '@mui/material/Rating';
import LikeDislikeButtons from './LikeDislikeButtons';
import { Button, Select, MenuItem, InputLabel, FormControl } from '@mui/material';

interface Review {
  id: number;
  username: string;
  comment: string;
  rating: number;
  date_posted: string;
  likeCount: number;
  dislikeCount: number;
}

interface Event {
  id: number;
  name: string;
  description: string;
  date_of_event: string;
  organizer: string;
  region: string;
  city: string;
  address: string;
  image: string;
  event_category: string;
  reviews: Review[];
  average_rating: number;
}

interface Profile {
  login: string;
  email: string;
  phone: string;
  nickname: string;
  image: string;
  worker: boolean;
  admin: boolean;
  visitorActions: number[];
  workerActions: number[];
}

const EventDetails: React.FC = () => {
  const { id } = useParams();
  const { login } = useUser();
  const navigate = useNavigate();

  const [event, setEvent] = useState<Event | null>(null);
  const [profile, setProfile] = useState<Profile | null>(null);
  const [newReview, setNewReview] = useState<Partial<Review>>({ username: '', comment: '', rating: 5 });
  const [editReview, setEditReview] = useState<Review | null>(null);
  const [isEditModalOpen, setIsEditModalOpen] = useState(false);
  const [isEditMode, setIsEditMode] = useState(false);
  const [updatedEvent, setUpdatedEvent] = useState<Partial<Event>>({});
  const [previewUrl, setPreviewUrl] = useState<string | null>(null);

  const dateOptions = {
    year: 'numeric',
    month: 'long',
    day: 'numeric',
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
    hour12: false, // 24-hour format
    timeZoneName: 'short'
  };

  useEffect(() => {
    const fetchEventDetails = async () => {
      try {
        const response = await fetch(`http://localhost:5000/events/${id}`);
        const data = await response.json();
        setEvent(data);
        setUpdatedEvent(data);
      } catch (error) {
        console.error('Error fetching event details:', error);
      }
    };

    const fetchProfile = async () => {
      try {
        const response = await fetch(`http://localhost:5000/profiles/${login}`);
        const data = await response.json();
        setProfile(data);
        setNewReview((prevReview) => ({ ...prevReview, username: data.nickname }));
      } catch (error) {
        console.error("Error fetching profiles:", error);
      }
    };

    fetchEventDetails();
    fetchProfile();
  }, [id, login]);

  const handleEditToggle = () => {
    setIsEditMode(!isEditMode);
  };

  const handleEventChange = (e: React.ChangeEvent<HTMLInputElement | HTMLTextAreaElement>) => {
    const { name, value } = e.target;
    setUpdatedEvent((prevEvent) => ({ ...prevEvent, [name]: value }));
  };

  const handleImageChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files[0];
    if (file) {
      const reader = new FileReader();
      reader.onloadend = () => {
        setUpdatedEvent((prevEvent) => ({ ...prevEvent, image: reader.result as string }));
        setPreviewUrl(reader.result as string);
      };
      reader.readAsDataURL(file);
    }
  };

  const handleSubmitEvent = async (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    try {
      const response = await fetch(`http://localhost:5000/events/${id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(updatedEvent),
      });
      if (response.ok) {
        const data = await response.json();
        setEvent(data);
        setIsEditMode(false);
        setPreviewUrl(null); // Clear the preview URL after saving
        navigate(`/event/${id}`); // Navigate back to the event details page
      } else {
        console.error('Failed to update event');
      }
    } catch (error) {
      console.error('Error updating event:', error);
    }
  };

  const handleSubmitReview = async (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    try {
      const response = await fetch(`http://localhost:5000/events/${id}/reviews`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(newReview),
      });
      if (response.ok) {
        const refreshResponse = await fetch(`http://localhost:5000/events/${id}`);
        const refreshedData = await refreshResponse.json();
        setEvent(refreshedData);
        setNewReview((prevReview) => ({ username: prevReview.username, comment: '', rating: 5 })); // Keep the username
        navigate(`/event/${id}`);
      }
    } catch (error) {
      console.error('Error submitting review:', error);
    }
  };

  const handleDeleteReview = async (reviewId: number) => {
    try {
      const response = await fetch(`http://localhost:5000/events/${id}/reviews/${reviewId}`, {
        method: 'DELETE',
      });
      if (response.ok) {
        const refreshResponse = await fetch(`http://localhost:5000/events/${id}`);
        const refreshedData = await refreshResponse.json();
        setEvent(refreshedData);
      } else {
        console.error('Failed to delete review');
      }
    } catch (error) {
      console.error('Error deleting review:', error);
    }
  };

  const handleEditReview = (review: Review) => {
    setEditReview(review);
    setIsEditModalOpen(true);
  };

  const handleUpdateReview = async (updatedReview: Review) => {
    try {
      const response = await fetch(`http://localhost:5000/events/${id}/reviews/${updatedReview.id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(updatedReview),
      });

      if (response.ok) {
        const refreshResponse = await fetch(`http://localhost:5000/events/${id}`);
        const refreshedData = await refreshResponse.json();

        setEvent((prevEvent) => {
          if (!prevEvent) return null;
          return {
            ...prevEvent,
            reviews: prevEvent.reviews.map((review) =>
              review.id === updatedReview.id ? updatedReview : review
            ),
          };
        });

        setIsEditModalOpen(false);
      }
    } catch (error) {
      console.error('Error updating review:', error);
    }
  };

  if (!event) return <div className="container mx-auto px-4 py-8">Načítání...</div>;

  return (
    <div className="container mx-auto px-4 py-8">
      <div className="bg-gray-200 rounded-lg shadow-lg p-6 mb-8 relative" style={{ maxWidth: '700px', margin: '0 auto' }}>
        {isEditMode ? (
          <form onSubmit={handleSubmitEvent}>
            {previewUrl && (
              <div>
                <img src={previewUrl} alt="Preview" style={{ height: "200px", width: "auto" }} />
              </div>
            )}
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Obrázek</label>
              <input
                type="file"
                accept="image/*"
                onChange={handleImageChange}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
              />
            </div>
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Název akce</label>
              <input
                type="text"
                name="name"
                value={updatedEvent.name || ''}
                onChange={handleEventChange}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Popis</label>
              <textarea
                name="description"
                value={updatedEvent.description || ''}
                onChange={handleEventChange}
                rows={4}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Datum konání</label>
              <input
                type="date"
                name="date_of_event"
                value={updatedEvent.date_of_event || ''}
                onChange={handleEventChange}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Organizátor</label>
              <input
                type="text"
                name="organizer"
                value={updatedEvent.organizer || ''}
                onChange={handleEventChange}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>
            <div className="mb-4">
              <FormControl fullWidth required>
                <InputLabel>Region</InputLabel>
                <Select
                  name="region"
                  value={updatedEvent.region || ''}
                  onChange={handleEventChange}
                >
                  <MenuItem value="praha">Praha</MenuItem>
                  <MenuItem value="stredocesky">Středočeský</MenuItem>
                  <MenuItem value="jihocesky">Jihočeský</MenuItem>
                  <MenuItem value="plzensky">Plzeňský</MenuItem>
                  <MenuItem value="karlovarsky">Karlovarský</MenuItem>
                  <MenuItem value="ustecky">Ústecký</MenuItem>
                  <MenuItem value="liberecky">Liberecký</MenuItem>
                  <MenuItem value="kralovehradecky">Královéhradecký</MenuItem>
                  <MenuItem value="pardubicky">Pardubický</MenuItem>
                  <MenuItem value="vysocina">Vysočina</MenuItem>
                  <MenuItem value="jihomoravsky">Jihomoravský</MenuItem>
                  <MenuItem value="olomoucky">Olomoucký</MenuItem>
                  <MenuItem value="moravskoslezsky">Moravskoslezský</MenuItem>
                  <MenuItem value="zlinsky">Zlínský</MenuItem>
                </Select>
              </FormControl>
            </div>
            <div className="mb-4">
              <FormControl fullWidth required>
                <InputLabel>Typ akce</InputLabel>
                <Select
                  name="event_category"
                  value={updatedEvent.event_category || ''}
                  onChange={handleEventChange}
                >
                  <MenuItem value="entertainment">Zábava</MenuItem>
                  <MenuItem value="education">Vzdělání</MenuItem>
                  <MenuItem value="for_adults">Pro dospělé</MenuItem>
                  <MenuItem value="for_children">Pro děti</MenuItem>
                  <MenuItem value="workshop">Workshop</MenuItem>
                  <MenuItem value="exposition">Výstava</MenuItem>
                </Select>
              </FormControl>
            </div>
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Město</label>
              <input
                type="text"
                name="city"
                value={updatedEvent.city || ''}
                onChange={handleEventChange}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>
            <div className="mb-4">
              <label className="block text-sm font-medium text-gray-700">Adresa</label>
              <input
                type="text"
                name="address"
                value={updatedEvent.address || ''}
                onChange={handleEventChange}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>
            <div className="flex justify-end">
              <Button type="submit" variant="contained" color="primary">Uložit</Button>
              <Button onClick={handleEditToggle} variant="outlined" color="secondary" className="ml-2">Zrušit</Button>
            </div>
          </form>
        ) : (
          <>
            <h2 className="text-3xl font-bold mb-2">{event.name}</h2>
            <div className="text-sm text-gray-600 mb-4 flex justify-between items-center">
              <p>{event.event_category}</p>
              <p>{event.date_of_event || 'Datum konání neznámo'}</p>
            </div>
            <div className="h-64 mb-4 rounded overflow-hidden">
              {event.image ? (
                <EventImage
                  eventId={event.id}
                  name={event.name}
                  className="w-full h-full object-cover"
                />
              ) : (
                <div className="w-full h-full flex items-center justify-center bg-gray-300 text-gray-500">
                  Obrázek není dostupný
                </div>
              )}
            </div>
            <p className="text-gray-800 text-sm mb-2">
              {event.description || "Popis události není k dispozici."}
            </p>
            <div className="my-4 bg-gray-100 p-4 rounded shadow-inner">
              <h3 className="text-lg font-bold">Organizátor:</h3>
              <p className="text-gray-600">{event.organizer || "Organizátor neznámy"}</p>
            </div>
            <div className="my-4 bg-gray-100 p-4 rounded shadow-inner">
              <h3 className="text-lg font-bold">Region:</h3>
              <p className="text-gray-600">{event.region || "Region neznámy"}</p>
            </div>
            <div className="my-4 bg-gray-100 p-4 rounded shadow-inner">
              <h3 className="text-lg font-bold">Město:</h3>
              <p className="text-gray-600">{event.city || "Město neznámo"}</p>
            </div>
            <div className="my-4 bg-gray-100 p-4 rounded shadow-inner">
              <h3 className="text-lg font-bold">Adresa:</h3>
              <p className="text-gray-600">{event.address || "Adresa neznáma"}</p>
            </div>
            {profile?.admin && (
              <div className="flex justify-end">
                <Button onClick={handleEditToggle} variant="contained" color="primary">Upravit</Button>
              </div>
            )}
          </>
        )}
      </div>

      {/* Reviews add */}
      <div className="bg-white rounded-lg shadow-lg p-6 mb-8">
        <h3 className="text-xl font-bold mb-4">Napsat recenzi</h3>
        <form onSubmit={handleSubmitReview} className="space-y-4 pl-64 pr-64 pt-10">
          <div className="bg-gray-200 p-5">
            <div>
              <label htmlFor="username" className="block text-sm font-medium text-gray-700">Uživatelské jméno</label>
              <p className="mt-1 block w-full rounded-md sm:text-sm">{newReview.username}</p>
            </div>

            <div>
              <label htmlFor="rating" className="block text-sm font-medium text-gray-700">Hodnocení</label>
              <Rating
                name="half-rating"
                value={newReview.rating}
                precision={0.5}
                onChange={(event, newValue) => setNewReview({ ...newReview, rating: newValue })}
              />
            </div>

            <div>
              <label htmlFor="comment" className="block text-sm font-medium text-gray-700">Komentář</label>
              <textarea
                id="comment"
                value={newReview.comment}
                onChange={(e) => setNewReview({ ...newReview, comment: e.target.value })}
                rows={4}
                className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                required
              />
            </div>

            <div className="flex justify-between mt-6 gap-10">
              <Link to={`/event/${id}`} className="text-white hover:text-indigo-200">
                <button className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400">Zpět</button>
              </Link>
              <button
                type="submit"
                className="bg-indigo-600 text-white py-2 px-4 rounded-lg hover:bg-indigo-700"
              >
                Zveřejnit recenzi
              </button>
            </div>
          </div>
        </form>
      </div>

      {/* Reviews list */}
      <h3 className="text-xl font-bold mb-4">Recenze ({event.reviews.length})</h3>
      <div className="mb-8">
        {event.reviews.map((review) => (
          <div key={review.id} className="border-b py-4 last:border-b-0">
            <div className="grid grid-cols-[70%,30%] gap-4 items-center">
              {/* Username and stars with date in the first row */}
              <p className="font-medium">{review.username}</p>
              <div className="flex justify-end">
                <div className="flex text-2xl pr-10">
                  <Rating name="read-only" value={review.rating} precision={0.5} readOnly />
                </div>
                <p className="text-xs m-2 text-gray-500">{new Date(review.date_posted).toLocaleDateString("cs-CZ", dateOptions)}</p>
              </div>
            </div>
            {/* Comment and buttons in the second row */}
            <p className="text-gray-600 break-words">{review.comment}</p>
            <div className="flex justify-end mt-2 space-x-2">
              <LikeDislikeButtons
                eventId={event.id}
                reviewId={review.id}
                initialLikes={Array.isArray(review.likeCount) ? review.likeCount : []}
                initialDislikes={Array.isArray(review.dislikeCount) ? review.dislikeCount : []}
                userLogin={login}   
              />
              <button className="bg-red-500 text-white py-1 px-3 rounded hover:bg-red-600" onClick={() => handleDeleteReview(review.id)}>Delete</button>
              <button className="bg-blue-500 text-white py-1 px-3 rounded hover:bg-blue-600" onClick={() => handleEditReview(review)}>Edit</button>
            </div>
          </div>
        ))}
      </div>
      <EditReviewModal review={editReview} isOpen={isEditModalOpen} onClose={() => setIsEditModalOpen(false)} onSave={handleUpdateReview} />
    </div>
  );
};

export default EventDetails;
