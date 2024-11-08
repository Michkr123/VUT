import React, { useState, useEffect } from 'react';

const EventImage = ({ eventId, name, className }) => {
  const [imageData, setImageData] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchImage = async () => {
      try {
        setLoading(true);
        const response = await fetch(`http://localhost:5000/events/${eventId}/image`);
        if (!response.ok) throw new Error('Failed to load image');
        const data = await response.json();
        setImageData(data.image);
      } catch (err) {
        console.error('Error loading image:', err);
        setError(err);
      } finally {
        setLoading(false);
      }
    };

    fetchImage();
  }, [eventId]);

  if (loading) {
    return (
      <div className={`flex items-center justify-center ${className} bg-gray-100`}>
        <div className="animate-spin rounded-full h-8 w-8 border-b-2 border-indigo-600"></div>
      </div>
    );
  }

  if (error || !imageData) {
    return (
      <div className={`flex items-center justify-center ${className} bg-gray-200`}>
        <span className="text-gray-500">Obrázek není dostupný</span>
      </div>
    );
  }

  return (
    <img
      src={imageData}
      alt={name}
      className={className}
    />
  );
};

export default EventImage;
