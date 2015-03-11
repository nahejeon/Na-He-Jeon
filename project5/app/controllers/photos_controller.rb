class PhotosController < ApplicationController
	def index
		@person = User.find(params[:id])
		@photos = @person.photos
	end
	def new
	end
	def create
		if session[:current_user_id] == nil then
			redirect_to "/users/login"
		else
			uploaded_io = params[:user][:photo]
			File.open(Rails.root.join("app", "assets", "images", uploaded_io.original_filename), "wb") do |file|
				file.write(uploaded_io.read)
			end
			redirect_to "/photos/index/" + session[:current_user_id].to_s
			newphoto = Photo.new()
			newphoto.user_id = session[:current_user_id]
			newphoto.date_time = DateTime.now
			newphoto.file_name = uploaded_io.original_filename
			newphoto.save()
		end
	end
end
