import React, { useState, useEffect } from "react";
import { useParams, useNavigate  } from 'react-router-dom';
import { Link } from "react-router-dom";

import EventImage from "./components/EventImage";

interface AddReviewPageProps { login: string; }

interface Profile {
    login: string;
    email: string;
    phone: string;
    nickname: string;
    image: string;
}

const AddReviewPage: React.FC<AddReviewPageProps> = ({ login }) => {
    const { id } = useParams();
    const navigate = useNavigate(); 
    const [event, setEvent] = useState<any>(null);
    const [profile, setProfile] = useState<Profile | null>(null);

    const [newReview, setNewReview] = useState({ username: '', comment: '', rating: 5 });
  
    useEffect(() => {
        const fetchProfile = async () => {
            try {
                const response = await fetch(`http://localhost:5000/profiles/${login}`);
                const data = await response.json();
                setProfile(data);
                if (data) {
                    setNewReview((prevReview) => ({
                        ...prevReview,
                        username: data.nickname,
                    }));
                }
            } catch (error) {
                console.error("Error fetching profiles:", error);
            }
        };

        fetchProfile();
    }, [login]);

    useEffect(() => {
        const fetchEventDetails = async () => {
            try {
                const response = await fetch(`http://localhost:5000/events/${id}`);
                const data = await response.json();
                setEvent(data);
            } catch (error) {
                console.error('Error fetching event details:', error);
            }
        };

        fetchEventDetails();
    }, [id]);

    const handleSubmitReview = async (e: React.FormEvent) => {
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
                setNewReview({ username: '', comment: '', rating: 5 });
                navigate(`/event/${id}`);
            }
        } catch (error) {
            console.error('Error submitting review:', error);
        }
    };

    return (
        <main>
            <div className="flex justify-center items-center">
                <div className="relative h-48 w-96 bg-gray-200 m-10">
                    {event?.image ? (
                        <EventImage
                            eventId={event?.id}
                            name={event?.name}
                            className="w-full h-full object-cover"
                        />
                    ) : (
                        <div className="absolute inset-0 flex items-center justify-center text-gray-500 bg-gray-200">
                            Obrázek není dostupný
                        </div>
                    )}

                    <div className="absolute top-4 left-4 bg-blue-600 text-white px-3 py-1 rounded">
                        {event?.name}
                    </div>
                </div>
            </div>

            <form onSubmit={handleSubmitReview} className="space-y-4 pl-64 pr-64 pt-10">
                <div>
                    <label htmlFor="username" className="block text-sm font-medium text-gray-700">
                        Uživatelské jméno
                    </label>
                    {/* Display the username as a static value */}
                    <p className="mt-1 block w-full rounded-md border-gray-300 shadow-sm sm:text-sm">
                        {newReview.username}
                    </p>
                </div>

                <div>
                    <label htmlFor="rating" className="block text-sm font-medium text-gray-700">
                        Hodnocení
                    </label>
                    <select
                        id="rating"
                        value={newReview.rating}
                        onChange={(e) => setNewReview({ ...newReview, rating: Number(e.target.value) })}
                        className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
                    >
                        <option key={5} value={5}>5 hvězdiček</option>
                        <option key={4} value={4}>4 hvězdičky</option>
                        <option key={3} value={3}>3 hvězdičky</option>
                        <option key={2} value={2}>2 hvězdičky</option>
                        <option key={1} value={1}>1 hvězdička</option>
                    </select>
                </div>

                <div>
                    <label htmlFor="comment" className="block text-sm font-medium text-gray-700">
                        Komentář
                    </label>
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
                        <button className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400">
                            Zpět
                        </button>
                    </Link>
                    <button
                        type="submit"
                        className="inline-flex justify-center rounded-md border border-transparent bg-indigo-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2"
                    >
                        Zveřejnit recenzi
                    </button>
                </div>
            </form>
        </main>
    );
};

export default AddReviewPage;
